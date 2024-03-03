#include "renderer.h"

#include "source/common/crash.h"
#include "source/rendering/render_config.h"
#include "source/rendering/transformations_buffer_iterator.h"

#include "source/common/common.h"
#include "source/window/window_manager.h"

#include "source/entities/component.h"
#include "source/components/camera.h"
#include "source/components/mesh.h"

#include "source/assets/shader_asset.h"
#include "source/assets/texture_asset.h"
#include "source/assets/mesh_asset.h"
#include "source/assets/sprite_sheet.h"
#include "source/assets/tileset_asset.h"

#include "graphics_abstraction/graphics_abstraction.h"
#include "opengl_3_3_api/opengl_3.3_api.h"

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <unordered_map>

using namespace rendering;

struct renderer::implementation
{
    struct geometry;

    graphics_abstraction::api* api = nullptr;

    graphics_abstraction::vertex_layout* transformations_buffer_layout = nullptr;

    graphics_abstraction::textures_set* textures = nullptr;

    graphics_abstraction::framebuffer* pre_postprocess_buffer = nullptr;
    graphics_abstraction::texture* pre_postprocess_buffer_color = nullptr;
    graphics_abstraction::texture* pre_postprocess_buffer_depth = nullptr;

    graphics_abstraction::shader* postprocess_shader = nullptr;
    graphics_abstraction::buffer* screen_quad_vertices = nullptr;
    graphics_abstraction::vertex_layout* screen_quad_vertices_layout = nullptr;

    std::unordered_map<render_config, geometry> pipelines;

    entities::components::camera* active_camera = nullptr;

    uint32_t current_uid = 0;

    static void resize()
    {
        auto& impl = common::renderer->impl;

        auto size = common::window_manager->get_size();
        
        impl->api->set_screen_size(size.first, size.second);
    }

    struct geometry
    {
        bool should_reload_transformations = false;
        std::vector<entities::components::mesh*> meshes;

        uint32_t visible_instances = 0;
        graphics_abstraction::buffer* transformations_buffer;

        bool should_resize_buffer = false;
        uint32_t new_buffer_size;
        
        geometry() {};
        geometry(geometry& othr) = delete;
        geometry(geometry&& othr)
        {
            this->meshes = std::move(othr.meshes);
            this->transformations_buffer = std::move(othr.transformations_buffer);
            othr.transformations_buffer = nullptr;
            should_reload_transformations = othr.should_reload_transformations;
        }

        ~geometry()
        {
            if (transformations_buffer != nullptr)
                common::renderer->impl->api->free(transformations_buffer);
        }
    };
};

renderer::renderer()
{
    impl = new implementation;
}

renderer::~renderer()
{
    impl->pipelines.clear();

    impl->api->free(impl->textures);

    impl->api->free(impl->pre_postprocess_buffer_color);
    impl->api->free(impl->pre_postprocess_buffer_depth);
    impl->api->free(impl->pre_postprocess_buffer);

    impl->api->free(impl->postprocess_shader);
    impl->api->free(impl->screen_quad_vertices);
    impl->api->free(impl->screen_quad_vertices_layout);

    delete impl->api;
}

void renderer::create_main_renderbuffer()
{
    auto fb = impl->api->create_framebuffer_builder();

    auto tb = impl->api->create_texture_builder();

    tb->texture_type = graphics_abstraction::texture_type::texture_2d;
    tb->internal_format = graphics_abstraction::texture_internal_format::rgb;
    tb->source_format = graphics_abstraction::texture_internal_format::unspecified;
    tb->width = 1920;
    tb->height = 1080;
    tb->generate_mipmaps = false;

    auto color_buffer = reinterpret_cast<graphics_abstraction::texture*>(impl->api->build(tb, false));

    impl->pre_postprocess_buffer_color = color_buffer;
    fb->color_buffers = { {0, color_buffer} };

    tb->texture_type = graphics_abstraction::texture_type::renderbuffer;
    tb->internal_format = graphics_abstraction::texture_internal_format::depth24_stencil8;

    fb->depth_stencil_buffer = static_cast<graphics_abstraction::texture*>(impl->api->build(tb));
    impl->pre_postprocess_buffer_depth = fb->depth_stencil_buffer;

    impl->pre_postprocess_buffer = reinterpret_cast<graphics_abstraction::framebuffer*>(impl->api->build(fb));
}

void renderer::initialize()
{
    impl->api = new graphics_abstraction::implementations::opengl_3_3_api::opengl_3_3_api;
    if (impl->api == nullptr)
        error_handling::crash(error_handling::error_source::core, "[renderer::initialize]",
            "Cannot initialize api instance");
    impl->textures = reinterpret_cast<graphics_abstraction::textures_set*>(
        impl->api->build(impl->api->create_textures_set_builder())
    );
    create_main_renderbuffer();

    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f, 1.0f,   1.0f, 1.0f
    };

    auto bb = impl->api->create_buffer_builder();
    bb->buffer_type = graphics_abstraction::buffer_type::vertex;
    bb->size = sizeof(vertices);
    impl->screen_quad_vertices = reinterpret_cast<graphics_abstraction::buffer*>(impl->api->build(bb));

    void* begin = impl->screen_quad_vertices->open_data_stream();
    memcpy(begin, &vertices[0], sizeof(vertices));
    impl->screen_quad_vertices->close_data_stream();

    auto vlb = impl->api->create_vertex_layout_builder();
    vlb->vertex_components = {
        graphics_abstraction::data_type::vec2,
        graphics_abstraction::data_type::vec2
    };
    impl->screen_quad_vertices_layout = reinterpret_cast<graphics_abstraction::vertex_layout*>(impl->api->build(vlb, false));

    vlb->vertex_components = {
        graphics_abstraction::data_type::vec2,  //Instance World Position
        graphics_abstraction::data_type::vec2,  //Instance Scale
        graphics_abstraction::data_type::Int,   //Instance Layer
        graphics_abstraction::data_type::Int    //Instance Sprite ID
    };
    impl->transformations_buffer_layout = reinterpret_cast<graphics_abstraction::vertex_layout*>(impl->api->build(vlb));

    std::string bypass_postprocess_code_ver = "#version 330 core\nlayout (location = 0) in vec2 aPos; layout (location = 1) in vec2 aTexCoord; out vec2 TexCoord; void main() {gl_Position = vec4(aPos, 0.0, 1.0); TexCoord = aTexCoord;}";
    std::string bypass_postprocess_code_frag = "#version 330 core\nout vec4 FragColor; in vec2 TexCoord; uniform sampler2D colorTexture; void main() { FragColor = texture(colorTexture, TexCoord); }";

    auto sb = impl->api->create_shader_builder();
    sb->code = {
        bypass_postprocess_code_ver,
        bypass_postprocess_code_frag
    };
    impl->postprocess_shader = reinterpret_cast<graphics_abstraction::shader*>(impl->api->build(sb));
}

void renderer::mark_pipeline_dirty(const render_config& pipeline)
{
    impl->pipelines.at(pipeline).should_reload_transformations = true;
}

void renderer::register_mesh_component(entities::components::mesh* mesh)
{
    auto itr = impl->pipelines.find(mesh->get_render_config());
    if (itr == impl->pipelines.end())
    {
        auto bb = impl->api->create_buffer_builder();
        //at default each pipeline can draw 64 meshes at once
        //if this initial buffer turns out to be to small new bigger buffer will be allocated
        //(see renderer::update_transformations)
        bb->size = impl->transformations_buffer_layout->get_vertex_size() * 64;
        bb->buffer_type = graphics_abstraction::buffer_type::instanced;

        implementation::geometry geo;
        geo.meshes = { mesh };
        geo.transformations_buffer = reinterpret_cast<graphics_abstraction::buffer*>(impl->api->build(bb));
        geo.should_reload_transformations = true;

        impl->pipelines.insert({ mesh->get_render_config(), std::move(geo) });
    }
    else
    {
        itr->second.meshes.push_back(mesh);
        itr->second.should_reload_transformations = true;
    }
}

void renderer::unregister_mesh_component(entities::components::mesh* mesh)
{
    auto p_itr = impl->pipelines.find(mesh->get_render_config());

    if (p_itr == impl->pipelines.end())
        return;

    auto m_itr = std::find(
        p_itr->second.meshes.begin(),
        p_itr->second.meshes.end(),
        mesh
    );

    p_itr->second.meshes.erase(m_itr);

    if (p_itr->second.meshes.size() == 0)
        impl->pipelines.erase(p_itr);
    else
        p_itr->second.should_reload_transformations = true;
}

void renderer::update_transformations()
{
    for (auto& pipeline : impl->pipelines)
    {
        if (!pipeline.second.should_reload_transformations)
            continue;
        pipeline.second.should_reload_transformations = false;

        if (pipeline.second.should_resize_buffer)
        {
            pipeline.second.transformations_buffer->reallocate(pipeline.second.new_buffer_size);
        }

        uint32_t buffer_size = pipeline.second.transformations_buffer->get_size();

        void* buffer_begin = pipeline.second.transformations_buffer->open_data_stream();
        transformations_buffer_iterator tbi{ buffer_begin };

        uint32_t instances_to_add = 0;

        for (auto& mesh : pipeline.second.meshes)
        {
            if ((buffer_size - tbi.get_data_size()) / impl->transformations_buffer_layout->get_vertex_size() < mesh->get_instances_amount())
                instances_to_add += mesh->get_instances_amount();
            else
                mesh->pass_transformation(tbi);
        }

        pipeline.second.visible_instances = tbi.get_data_size() / impl->transformations_buffer_layout->get_vertex_size();
        pipeline.second.transformations_buffer->close_data_stream();

        if (instances_to_add != 0)
        {
            pipeline.second.should_resize_buffer = true;
            pipeline.second.new_buffer_size = int((pipeline.second.visible_instances + instances_to_add) + 32) *
                impl->transformations_buffer_layout->get_vertex_size();
            pipeline.second.should_reload_transformations = true;
        }
    }
}

void renderer::render()
{
    if (impl->active_camera == nullptr)
        return;

    auto screen_framebuffer = impl->pre_postprocess_buffer;
    impl->api->bind(screen_framebuffer);
    impl->api->set_screen_size(1920, 1080);

    impl->api->set_enabled(graphics_abstraction::functionalities::depth_testing, true);

    screen_framebuffer->clear_color_buffers(0.0f, 0.0f, 0.0f, 1.0f);
    screen_framebuffer->clear_depth_buffer();
    screen_framebuffer->clear_stencil_buffer();

    impl->api->bind(impl->textures);

    auto projection = impl->active_camera->get_projection();
    auto camera_view_center_v2 = impl->active_camera->get_view_center_location();
    glm::vec4 camera_view_center_v4 = { camera_view_center_v2.x, camera_view_center_v2.y , 0, 0 };
    int lowest_layer = impl->active_camera->lowest_layer;
    int highest_layer = impl->active_camera->highest_layer;

    for (auto& pipeline : impl->pipelines)
    {
        if (pipeline.first.material != nullptr)
        {
            pipeline.first.material->_shader->set_uniform_value(
                "itr_projection", graphics_abstraction::data_type::mat4x4, glm::value_ptr(projection));
            pipeline.first.material->_shader->set_uniform_value(
                "itr_camera_location", graphics_abstraction::data_type::vec4, glm::value_ptr(camera_view_center_v4));
            pipeline.first.material->_shader->set_uniform_value(
                "itr_lowest_layer", graphics_abstraction::data_type::Int, &lowest_layer);
            pipeline.first.material->_shader->set_uniform_value(
                "itr_highest_layer", graphics_abstraction::data_type::Int, &highest_layer);

            if (pipeline.first.textures.size() != 0)
            {
                glm::vec2 sprites_count = { 1, 1 };

                auto as_sprite_sheet = dynamic_cast<assets::sprite_sheet*>(pipeline.first.textures.at(0).get());
                auto as_tileset = dynamic_cast<assets::tileset*>(pipeline.first.textures.at(0).get());
                if (as_sprite_sheet != nullptr)
                    sprites_count = {
                        as_sprite_sheet->get_width()  / as_sprite_sheet->sprite_width,
                        as_sprite_sheet->get_height() / as_sprite_sheet->sprite_height
                    };
                else if (as_tileset != nullptr)
                    sprites_count = {
                        as_tileset->get_width()  / as_tileset->tile_width,
                        as_tileset->get_height() / as_tileset->tile_height
                    };

                pipeline.first.material->_shader->set_uniform_value(
                    "itr_sprites", graphics_abstraction::data_type::vec2, glm::value_ptr(sprites_count));
            }

            impl->api->bind(pipeline.first.material->_shader);
            impl->api->bind(pipeline.first.material->vertex_layout);
            impl->api->bind(pipeline.first.mesh->vertices);
            if (pipeline.first.mesh->indicies != nullptr)
                impl->api->bind(pipeline.first.mesh->indicies);

            impl->api->bind(pipeline.second.transformations_buffer, impl->transformations_buffer_layout);

            std::vector<graphics_abstraction::texture*> textures;

            for (auto& txt : pipeline.first.textures)
                textures.push_back(txt->_texture);

            impl->textures->set_selection(textures);

            impl->api->apply_bindings();

            if (pipeline.first.mesh->draw_type == assets::mesh::draw_type::indexed_triangles)
            {
                impl->api->draw(graphics_abstraction::draw_args{
                    graphics_abstraction::draw_types::instanced_indexed,
                        graphics_abstraction::primitives::triangle,
                        graphics_abstraction::draw_args::instanced_indexed_draw_args
                    {
                        pipeline.first.mesh->indicies->get_size() / sizeof(int), (uint32_t)pipeline.second.meshes.size()
                    }
                });
            }
            else
            {
                impl->api->draw(graphics_abstraction::draw_args{
                    graphics_abstraction::draw_types::instanced_array,
                        graphics_abstraction::primitives::triangle_strip,
                        graphics_abstraction::draw_args::instanced_array_draw_args
                    {
                        graphics_abstraction::draw_args::array_draw_args{
                            0, 
                            pipeline.first.mesh->vertices->get_size() / pipeline.first.material->vertex_layout->get_vertex_size()
                        }
                        , pipeline.second.visible_instances
                    }
                });
            }
        }
    }
    
    auto size = common::window_manager->get_size();

    impl->api->set_screen_size(size.first, size.second);
    impl->api->bind(impl->api->get_default_framebuffer());
    impl->api->bind(impl->postprocess_shader);
    impl->api->bind(impl->screen_quad_vertices);
    impl->api->bind(impl->screen_quad_vertices_layout);

    impl->textures->set_selection({ impl->pre_postprocess_buffer_color });

    impl->api->set_enabled(graphics_abstraction::functionalities::depth_testing, false);

    impl->api->apply_bindings();

    impl->api->draw(graphics_abstraction::draw_args{
        graphics_abstraction::draw_types::array,
        graphics_abstraction::primitives::triangle_strip,
        graphics_abstraction::draw_args::array_draw_args
        {
            0, 4
        }
    });
}

graphics_abstraction::api* renderer::get_api()
{
    return impl->api;
}

void renderer::set_active_camera(entities::components::camera* camera)
{
    impl->active_camera = camera;
}

entities::components::camera* renderer::get_active_camera()
{
    return impl->active_camera;
}

std::function<void(void)> renderer::get_resize_function()
{
    return std::function<void(void)>{implementation::resize};
}

uint32_t renderer::get_new_uid()
{
    if (impl->current_uid == UINT32_MAX)
        impl->current_uid = 0;
    impl->current_uid++;
    return impl->current_uid;
}