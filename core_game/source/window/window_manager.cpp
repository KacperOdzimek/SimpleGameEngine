#include "window_manager.h"

#include "source/common/common.h"
#include "source/common/crash.h"

#include "glfw/glfw3.h"

using namespace window;

struct window_manager::implementation
{
	GLFWwindow* window;
    int width;
    int height;
    bool fullscreen;

    std::function<void(void)> resize_callback;

    static void glfw_window_resize_callback(GLFWwindow* _window, int _width, int _height)
    {
        common::window_manager->impl->width = _width;
        common::window_manager->impl->height = _height;

        if (common::window_manager->impl->resize_callback)
            common::window_manager->impl->resize_callback();
    }
};

window_manager::window_manager()
{
    impl = new implementation;
}

window_manager::~window_manager()
{
    delete impl;
    glfwTerminate();
}

void window_manager::create_window(std::string title, int width, int height, bool fullscreen)
{
    if (glfwInit() == GLFW_FALSE)
        error_handling::crash(error_handling::error_source::core, "[renderer::initialize]",
            "Cannot create window");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    impl->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    impl->width = width;
    impl->height = height;
    impl->fullscreen = fullscreen;

    if (impl->window == nullptr)
        error_handling::crash(error_handling::error_source::core, "[renderer::initialize]",
            "Cannot create window");

    glfwMakeContextCurrent(impl->window);
    glfwSetFramebufferSizeCallback(impl->window, &implementation::glfw_window_resize_callback);
    glfwSetWindowAspectRatio(impl->window, 16, 9);
    glfwSwapInterval(1);
}

void window_manager::change_frame()
{
    glfwPollEvents();
    glfwSwapBuffers(impl->window);
}

bool window_manager::should_close()
{
    return glfwWindowShouldClose(impl->window);;
}

std::pair<int, int> window_manager::get_size()
{
    return { impl->width, impl->height };
}

void window_manager::set_resize_callback(std::function<void()> callback)
{
    impl->resize_callback = callback;
}

input::key_state window_manager::get_key_state(input::key key)
{
    input::key_state ks;
    ks.key = key;

    if (key.key_type == input::key_type::keyboard || key.key_type == input::key_type::mouse)
        ks.state = (glfwGetKey(impl->window, key.id) == GLFW_PRESS || glfwGetKey(impl->window, key.id) == GLFW_REPEAT);
    else
        ks.state = 0;
    return ks;
}