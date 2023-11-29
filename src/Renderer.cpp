#include "Renderer.h"
#include "Device.h"
#include "GLFW/glfw3.h"
#include "Logger.h"
#include "Swapchain.h"
#include "Window.h"
#include "vulkan/vulkan_core.h"
#include <cassert>
#include <memory>
#include <string>

Renderer::Renderer(Window &window, VKDevice &device)
: WINDOW{window}, DEVICE{device} {
    recreateSwapChain();
    createCommandBuffers();
}

Renderer::~Renderer() { freeCommandBuffers(); }

void Renderer::recreateSwapChain() {
    auto extent = WINDOW.getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = WINDOW.getExtent();
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(DEVICE.device());

    if (SWAPCHAIN == nullptr) {
        SWAPCHAIN = std::make_unique<Swapchain>(DEVICE, extent);
    } else {
        SWAPCHAIN = std::make_unique<Swapchain>(DEVICE, extent, std::move(SWAPCHAIN));
        if (SWAPCHAIN->imageCount() != commandBuffers.size()) {
            freeCommandBuffers();
            createCommandBuffers();
        }
    }
}

void Renderer::createCommandBuffers() {
    commandBuffers.resize(SWAPCHAIN->imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = DEVICE.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(DEVICE.device(), &allocInfo, commandBuffers.data()) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void Renderer::freeCommandBuffers() {
    vkFreeCommandBuffers(
        DEVICE.device(),
        DEVICE.getCommandPool(),
        static_cast<uint32_t>(commandBuffers.size()),
        commandBuffers.data()
    );

    commandBuffers.clear();
}

VkCommandBuffer Renderer::beginFrame() {
    assert(!isFrameStarted && "Cannot begin frame, another is already in progress");
    auto result = SWAPCHAIN->acquireNextImage(&currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    isFrameStarted = true;

    auto commandBuffer = getCurrentCommandBuffer();
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    return commandBuffer;
};

void Renderer::endFrame() {
    assert(isFrameStarted && "Cannot end frame, none have began");
    auto commandBuffer = getCurrentCommandBuffer();

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

    auto result = SWAPCHAIN->submitCommandBuffers(&commandBuffer, &currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        WINDOW.wasWindowResized()) {
        WINDOW.resetWindowResizedFlag();
        recreateSwapChain();
        YorkLogger::info("Resize: " + std::to_string(WINDOW.width) + "x" + std::to_string(WINDOW.height));
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    frame++;
    isFrameStarted = false;
    currentImageIndex = (currentImageIndex + 1) % Swapchain::MAX_FRAMES_IN_FLIGHT;
};

void Renderer::beginSwapchainRenderPass(VkCommandBuffer commandBuffer) {
    assert(isFrameStarted && "Cannot begin render pass, no frames are in progress");
    assert(commandBuffer == getCurrentCommandBuffer() && "Cannot begin render pass on command buffer from another frame");

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = SWAPCHAIN->getRenderPass();
    renderPassInfo.framebuffer = SWAPCHAIN->getFrameBuffer(currentImageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = SWAPCHAIN->getSwapChainExtent();

    VkClearValue clearValues[2];
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(sizeof(clearValues));
    renderPassInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(SWAPCHAIN->getSwapChainExtent().width);
    viewport.height = static_cast<float>(SWAPCHAIN->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, SWAPCHAIN->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
};

void Renderer::endSwapchainRenderPass(VkCommandBuffer commandBuffer) {
    assert(isFrameStarted && "Cannot begin render pass, no frames are in progress");
    assert(commandBuffer == getCurrentCommandBuffer() && "Cannot begin render pass on command buffer from another frame");
    vkCmdEndRenderPass(commandBuffer);
};