#include "Window.h"
#include "GLFW/glfw3.h"
#include "Logger.h"
#include "vulkan/vulkan_core.h"
#include <stdexcept>
#include <string>

Window::Window(int w, int h, std::string n) : width(w), height(h), name(n) {
    init();
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::init() {
    glfwInit();
    YorkLogger::info("Initialized window");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface");
    }
}

void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto glfwWindow = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    glfwWindow->framebufferResized = true;
    glfwWindow->width = width;
    glfwWindow->height = height;
}