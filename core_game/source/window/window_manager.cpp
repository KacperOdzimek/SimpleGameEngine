#include "window_manager.h"

#include "source/common/common.h"
#include "source/common/crash.h"

#include "glfw/glfw3.h"

#include <thread>
#include <chrono>

#include <iostream>

using namespace window;

struct window_manager::implementation
{
	GLFWwindow* window;
    int width;
    int height;

    bool fullscreen;
    float fullscreen_refresh_rate;
    int width_before_fullscreen;
    int height_before_fullscreen;
    int posx_before_fullscreen;
    int posy_before_fullscreen;

    bool previous_f11_key_state = 0;

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

GLFWmonitor* get_current_monitor(GLFWwindow* window);

void window_manager::update()
{
    glfwPollEvents();
    glfwSwapBuffers(impl->window);
    if (glfwGetKey(impl->window, GLFW_KEY_F11) == GLFW_PRESS && !impl->previous_f11_key_state)
    {
        impl->previous_f11_key_state = 1;
        impl->fullscreen = !impl->fullscreen;
        if (impl->fullscreen)
        {
            impl->width_before_fullscreen = impl->width;
            impl->height_before_fullscreen = impl->height;

            glfwGetWindowPos(impl->window, &impl->posx_before_fullscreen, &impl->posy_before_fullscreen);

            GLFWmonitor* monitor = get_current_monitor(impl->window);
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
            glfwSetWindowMonitor(impl->window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

            glfwSwapInterval(0);    //Disable vsync, because it does not work in fullscreen on many graphics drivers
                                    //Insted the refresh rate is adjusted in window_manager::vsync function

            impl->fullscreen_refresh_rate = static_cast<float>(mode->refreshRate);
        }
        else
        {
            glfwSetWindowMonitor(impl->window, NULL, 
                impl->posx_before_fullscreen, impl->posy_before_fullscreen, 
                impl->width_before_fullscreen, impl->height_before_fullscreen, 
                GLFW_DONT_CARE);
            glfwSwapInterval(1);
        }
    }
    else
        impl->previous_f11_key_state = glfwGetKey(impl->window, GLFW_KEY_F11) == GLFW_PRESS;
}

bool window_manager::should_close()
{
    return glfwWindowShouldClose(impl->window);;
}

std::pair<int, int> window_manager::get_size()
{
    return { impl->width, impl->height };
}

void window::window_manager::set_mouse_visible(bool visible)
{
    if (visible)
        glfwSetInputMode(impl->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
        glfwSetInputMode(impl->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void window_manager::set_resize_callback(std::function<void()> callback)
{
    impl->resize_callback = callback;
}

void window_manager::vsync(double frame_processing_time)
{
    if (!impl->fullscreen) return;
    return;
    double frame_time = (1.0 / impl->fullscreen_refresh_rate);
    double sleep_time = frame_time - frame_processing_time;

    if (sleep_time > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(int(sleep_time * 1000)));
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

//Credit: https://stackoverflow.com/questions/21421074/how-to-create-a-full-screen-window-on-the-current-monitor-with-glfw
GLFWmonitor* get_current_monitor(GLFWwindow* window)
{
    int nmonitors, i;
    int wx, wy, ww, wh;
    int mx, my, mw, mh;
    int overlap, bestoverlap;
    GLFWmonitor* bestmonitor;
    GLFWmonitor** monitors;
    const GLFWvidmode* mode;

    bestoverlap = 0;
    bestmonitor = NULL;

    glfwGetWindowPos(window, &wx, &wy);
    glfwGetWindowSize(window, &ww, &wh);
    monitors = glfwGetMonitors(&nmonitors);

    for (i = 0; i < nmonitors; i++) {
        mode = glfwGetVideoMode(monitors[i]);
        glfwGetMonitorPos(monitors[i], &mx, &my);
        mw = mode->width;
        mh = mode->height;

        overlap =
            std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx)) *
            std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

        if (bestoverlap < overlap) {
            bestoverlap = overlap;
            bestmonitor = monitors[i];
        }
    }

    return bestmonitor;
}