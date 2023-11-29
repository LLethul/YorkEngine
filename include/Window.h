#ifndef WINDOW_H_
#define WINDOW_H_

#include "vulkan/vulkan_core.h"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <string>

using namespace std;

class Window {
    public:
    int width;
    int height;
    bool framebufferResized = false;

    const std::string name;
    GLFWwindow* window;
    
    Window(int w, int h, std::string n);
    ~Window();

    Window(const Window&) = delete;
    Window &operator=(const Window&) = delete;

    bool wasWindowResized() { return framebufferResized; }
    void resetWindowResizedFlag() { framebufferResized = false; }

    inline bool shouldClose() { return glfwWindowShouldClose(window); };
    inline VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    
    private:
    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
    void init();
};

#endif // WINDOW_H_