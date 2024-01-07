#include "renderer.h"

#include "graphics_abstraction/graphics_abstraction.h"
#include "opengl_3_3_api/opengl_3.3_api.h"
#include "glfw/glfw3.h"

#include "common/abort.h"
#include "source/rendering/renderer_data_types.h"
#include "source/rendering/buffer_iterator.h"
#include "source/entities/component.h"
#include "source/entities/geometry_component.h"

using namespace rendering;

struct renderer::implementation
{
    int window_width = 800;
    int window_height = 800;
    GLFWwindow* window;

    graphics_abstraction::api* api;
    pipelines pipelines;
    geometry_sources geometry_sources;
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
    //glfwSetFramebufferSizeCallback(impl->window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(impl->window, mouse_callback);
    //glfwSetScrollCallback(impl->window, scroll_callback);
    glfwSetInputMode(impl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);
}

void renderer::create_api_instance()
{
    impl->api = new graphics_abstraction::implementations::opengl_3_3_api::opengl_3_3_api;
    if (impl->api == nullptr)
        abort("Cannot create api instance");
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

        buffer_iterator ver_iter{ ver_beg };
        buffer_iterator ind_iter{ ind_beg };

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
    auto screen_framebuffer = impl->api->get_default_framebuffer();
    impl->api->bind(screen_framebuffer);
    screen_framebuffer->clear_color_buffers(0.0f, 0.2f, 0.1f, 1.0f);
    screen_framebuffer->clear_depth_buffer();
    screen_framebuffer->clear_stencil_buffer();

    for (auto& pipeline : impl->pipelines)
    {
        if (pipeline.first.shader != nullptr)
        {
            impl->api->bind(pipeline.first.shader->_shader);
            impl->api->bind(pipeline.first.shader->vertex_layout);
            impl->api->bind(pipeline.second.vertices);
            impl->api->bind(pipeline.second.indicies);

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
}

graphics_abstraction::api* renderer::get_api()
{
    return impl->api;
}