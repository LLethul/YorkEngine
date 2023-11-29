#ifndef RENDERER_H_
#define RENDERER_H_

#include "Window.h"
#include "Swapchain.h"
#include "Device.h"
#include "Global.h"
#include "vulkan/vulkan_core.h"
#include <cassert>
#include <ctime>
#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

struct VkBasicPushConstantData {
    glm::mat2 transform{1.f};
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

class Renderer {
    public:
    Renderer(Window &window, VKDevice &device);
    ~Renderer();

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    VkCommandBuffer beginFrame();
    void endFrame();

    void beginSwapchainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapchainRenderPass(VkCommandBuffer commandBuffer);

    VkRenderPass getSwapchainRenderPass() const { return SWAPCHAIN->getRenderPass(); };
    bool isFrameInProgress() const { return isFrameStarted; };
    VkCommandBuffer getCurrentCommandBuffer() const { assert(isFrameStarted && "Frame in progress!!"); return commandBuffers[currentImageIndex]; };

    private:
    void freeCommandBuffers();
    void createCommandBuffers();
    void recreateSwapChain();

    clock_t deltaTime = 0;
    int frames = 0;
    int frame = 0;

    Window &WINDOW;
    VKDevice &DEVICE;
    std::unique_ptr<Swapchain> SWAPCHAIN;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex;
    bool isFrameStarted = false;
};

#endif // RENDERER_H_