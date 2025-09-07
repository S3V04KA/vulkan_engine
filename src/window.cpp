#include "window.hpp"

#include <stdexcept>

namespace hex
{
    Window::Window(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
    {
        initWindow();
    }

    Window::~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Window::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface");
        }
    }

    void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        auto appWindow = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        appWindow->framebufferResized = true;
        appWindow->width = width;
        appWindow->height = height;
    }
}