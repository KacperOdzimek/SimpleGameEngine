#include "renderer.h"

#include "graphics_abstraction/graphics_abstraction.h"
#include "opengl_3_3_api/opengl_3.3_api.h"
#include "glfw/glfw3.h"

#include <glm/gtc/type_ptr.hpp>

#include "source/common/abort.h"
#include "source/rendering/renderer_data_types.h"
#include "source/rendering/buffer_iterator.h"

#include "source/entities/component.h"
#include "source/entities/geometry_component.h"
#include "source/components/camera.h"

using namespace rendering;

struct renderer::implementation
{
    int window_width = 16 * 80;
    int window_height = 9 * 80;
    GLFWwindow* window;

    graphics_abstraction::api* api = nullptr;;

    graphics_abstraction::textures_set* textures = nullptr;;

    graphics_abstraction::framebuffer* pre_postprocess_buffer = nullptr;;
    graphics_abstraction::texture* pre_postprocess_buffer_color = nullptr;;
    graphics_abstraction::texture* pre_postprocess_buffer_depth = nullptr;;

    graphics_abstraction::shader* postprocess_shader = nullptr;;
    graphics_abstraction::buffer* screen_quad_vertices = nullptr;;
    graphics_abstraction::vertex_layout* screen_quad_vertices_layout = nullptr;

    pipelines pipelines;
    geometry_sources geometry_sources;

    entities::components::camera* active_camera = nullptr;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        auto& impl = common::renderer->impl;

        impl->api->set_screen_size(width, height);
        impl->window_width = width;
        impl->window_height = height;

        impl->pre_postprocess_buffer_color->resize(width, height);
        impl->pre_postprocess_buffer_depth->resize(width, height);
    }
};

renderer::renderer()
{
    impl = new implementation;
}

renderer::~renderer()
{
    for (auto& pipeline : impl->pipelines)
    {
        impl->api->free(pipeline.second.vertices);
        impl->api->free(pipeline.second.indicies);
    }

    impl->api->free(impl->textures);

    impl->api->free(impl->pre_postprocess_buffer_color);
    impl->api->free(impl->pre_postprocess_buffer_depth);
    impl->api->free(impl->pre_postprocess_buffer);

    impl->api->free(impl->postprocess_shader);
    impl->api->free(impl->screen_quad_vertices);
    impl->api->free(impl->screen_quad_vertices_layout);

    delete impl->api;

    glfwTerminate();
}

void renderer::create_window()
{
    //We are using opengl
    if (glfwInit() == GLFW_FALSE)
        abort("Cannot create window");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    impl->window = glfwCreateWindow(impl->window_width, impl->window_height, "Top Down Game", NULL, NULL);
    if (impl->window == nullptr)
        abort("Cannot create window");
    glfwMakeContextCurrent(impl->window);
    glfwSetFramebufferSizeCallback(impl->window, implementation::framebuffer_size_callback);
    //glfwSetCursorPosCallback(impl->window, mouse_callback);
    //glfwSetScrollCallback(impl->window, scroll_callback);
    //glfwSetInputMode(impl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowAspectRatio(impl->window, 16, 9);
    glfwSwapInterval(1);
}

void renderer::create_main_renderbuffer()
{
    auto fb = impl->api->create_framebuffer_builder();

    auto tb = impl->api->create_texture_builder();

    tb->texture_type = graphics_abstraction::texture_type::texture_2d;
    tb->internal_format = graphics_abstraction::texture_internal_format::rgb;
    tb->source_format = graphics_abstraction::texture_internal_format::unspecified;
    tb->width = impl->window_width;
    tb->height = impl->window_height;
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

void renderer::create_api_instance()
{
    impl->api = new graphics_abstraction::implementations::opengl_3_3_api::opengl_3_3_api;
    if (impl->api == nullptr)
        abort("Cannot create api instance");
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
    impl->screen_quad_vertices_layout = reinterpret_cast<graphics_abstraction::vertex_layout*>(impl->api->build(vlb));

    std::string bypass_postprocess_code_ver = "#version 330 core\nlayout (location = 0) in vec2 aPos; layout (location = 1) in vec2 aTexCoord; out vec2 TexCoord; void main() {gl_Position = vec4(aPos, 0.0, 1.0); TexCoord = aTexCoord;}";
    std::string bypass_postprocess_code_frag = "#version 330 core\nout vec4 FragColor; in vec2 TexCoord; uniform sampler2D colorTexture; void main() { FragColor = texture(colorTexture, TexCoord); }";

    auto sb = impl->api->create_shader_builder();
    sb->code = {
        bypass_postprocess_code_ver,
        bypass_postprocess_code_frag
    };
    impl->postprocess_shader = reinterpret_cast<graphics_abstraction::shader*>(impl->api->build(sb));
}

bool renderer::should_window_close()
{
    return glfwWindowShouldClose(impl->window);
}

void renderer::update_window()
{
    glfwPollEvents();
    glfwSwapBuffers(impl->window);
}

void renderer::register_geometry_component(entities::geometry_component* comp, pipeline_config target_pipeline)
{
    if (impl->geometry_sources.find(target_pipeline) != impl->geometry_sources.end())
        impl->geometry_sources.at(target_pipeline).push_back(comp);
    else
    {
        impl->geometry_sources.insert({ target_pipeline, {comp} });

        //Alloc 4kb vertices buffer
        auto bb = impl->api->create_buffer_builder();
        bb->buffer_type = graphics_abstraction::buffer_type::vertex;
        bb->size = 1024 * sizeof(float);
        auto vertices = reinterpret_cast<graphics_abstraction::buffer*>(impl->api->build(bb, false));

        //Alloc 2kb vertices buffer
        bb->buffer_type = graphics_abstraction::buffer_type::indicies;
        bb->size = 512 * sizeof(int);
        auto indicies = reinterpret_cast<graphics_abstraction::buffer*>(impl->api->build(bb));
   
        pipeline_buffers pt;
        pt.vertices = vertices;
        pt.indicies = indicies;
        impl->pipelines.insert({ target_pipeline, {vertices, indicies} });
    }
}

void renderer::unregister_geometry_component(entities::geometry_component* comp, pipeline_config target_pipeline)
{
    #define target impl->geometry_sources.at(target_pipeline)
    if (impl->geometry_sources.find(target_pipeline) != impl->geometry_sources.end())
        target.erase(std::remove(target.begin(), target.end(), comp), target.end());
    #undef target
}

void renderer::collect_geometry_data()
{
    for (auto& x : impl->geometry_sources)
    {
        auto& buffers = impl->pipelines.at(x.first);
       
        void* ver_beg = buffers.vertices->open_data_stream();
        void* ind_beg = buffers.indicies->open_data_stream();

        vertices_buffer_iterator ver_iter{ ver_beg };
        indicies_buffer_iterator ind_iter{ ind_beg };

        for (auto& component : x.second)
        {
            component->push_geometry(ver_iter, ind_iter);
        }

        buffers.vertices->close_data_stream();
        buffers.indicies->close_data_stream();
        buffers.valid_indicies = ind_iter.get_data_size() / sizeof(int);
    }
}

void renderer::render()
{
    if (impl->active_camera == nullptr)
        return;

    auto screen_framebuffer = impl->pre_postprocess_buffer;
    impl->api->bind(screen_framebuffer);
    screen_framebuffer->clear_color_buffers(0.0f, 0.2f, 0.1f, 1.0f);
    screen_framebuffer->clear_depth_buffer();
    screen_framebuffer->clear_stencil_buffer();
    impl->api->bind(impl->textures);

    auto projection = impl->active_camera->get_projection();
    auto camera_view_center_v2 = impl->active_camera->get_view_center_location();
    glm::vec4 camera_view_center_v4 = { camera_view_center_v2.x, camera_view_center_v2.y , 0, 0 };

    for (auto& pipeline : impl->pipelines)
    {
        if (pipeline.first.shader != nullptr)
        {
            pipeline.first.shader->_shader->set_uniform_value(
                "itr_projection", graphics_abstraction::data_type::mat4x4, glm::value_ptr(projection));
            pipeline.first.shader->_shader->set_uniform_value(
                "itr_camera_location", graphics_abstraction::data_type::vec4, glm::value_ptr(camera_view_center_v4));

            impl->api->bind(pipeline.first.shader->_shader);
            impl->api->bind(pipeline.first.shader->vertex_layout);
            impl->api->bind(pipeline.second.vertices);
            impl->api->bind(pipeline.second.indicies);

            impl->textures->set_selection(
                pipeline.first.textures
            );

            impl->api->apply_bindings();

            impl->api->draw(graphics_abstraction::draw_args{
                graphics_abstraction::draw_types::indexed,
                graphics_abstraction::primitives::triangle,
                graphics_abstraction::draw_args::indexed_draw_args
                {
                    pipeline.second.valid_indicies
                }
            });
        }
    }

    impl->api->bind(impl->api->get_default_framebuffer());
    impl->api->bind(impl->postprocess_shader);
    impl->api->bind(impl->screen_quad_vertices);
    impl->api->bind(impl->screen_quad_vertices_layout);

    impl->textures->set_selection({ impl->pre_postprocess_buffer_color });
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