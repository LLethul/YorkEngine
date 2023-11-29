#include "App.h"
#include "GLFW/glfw3.h"
#include "GameObject.h"
#include "Logger.h"
#include "Model.h"
#include "Pipeline.h"
#include "Swapchain.h"
#include "Window.h"
#include "glm/common.hpp"
#include "vulkan/vulkan_core.h"
#include "glm/gtc/constants.hpp"
#include <ctime>
#include <memory>
#include <sstream>
#include <string>

App::App() {
    loadGameObjects();
    createPipelineLayout();
    createPipeline();
}

App::~App() { vkDestroyPipelineLayout(DEVICE.device(), pipelineLayout, nullptr); }

int App::run() {
    YorkLogger::info("Running app");
    YorkLogger::info(std::string(DEVICE.properties.deviceName));
    while (!WINDOW.shouldClose()) {
        glfwPollEvents();

        if (auto commandBuffer = RENDERER.beginFrame()) {
            RENDERER.beginSwapchainRenderPass(commandBuffer);
            renderGameObjects(commandBuffer);
            RENDERER.endSwapchainRenderPass(commandBuffer);
            RENDERER.endFrame();
        }
    }

    YorkLogger::info("Await " + std::string(DEVICE.properties.deviceName) + " to be freed");
    vkDeviceWaitIdle(DEVICE.device());
    glfwDestroyWindow(WINDOW.window);
    return 0;
}

void App::createPipelineLayout() {
    VkPushConstantRange pcRange{
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        .offset = 0,
        .size = sizeof(VkBasicPushConstantData)
    };

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pcRange;
    if (vkCreatePipelineLayout(DEVICE.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void App::createPipeline() {
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
    VKPipelineConfigurationInformation pipelineConfig {};
    VKPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = RENDERER.getSwapchainRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    PIPELINE = std::make_unique<VKPipeline>(
        DEVICE,
        "shaders/out/basic.vert.spv",
        "shaders/out/basic.frag.spv",
        pipelineConfig);
}

void App::loadGameObjects() {
    YorkLogger::info("Loading model");
    std::vector<Model::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
    auto model = std::make_shared<Model>(DEVICE, vertices);

    auto triangle = GameObject::New();
    triangle.model = model;
    triangle.transform2d.scale = {2.f, .4f};
    triangle.transform2d.rotation = .25f * glm::pi<float>() * .025f;
    triangle.color = {.1f, .8f, .1f};
    gameObjects.push_back(std::move(triangle));
}

void App::renderGameObjects(VkCommandBuffer commandBuffer) {
    PIPELINE->bind(commandBuffer);

    int i = 0;
    for (auto& obj : gameObjects) {
        i += 1;
        
        obj.transform2d.rotation =
            glm::mod<float>(obj.transform2d.rotation + 0.001f * i, 2.f * glm::pi<float>());
    }

    for (auto &obj : gameObjects) {
        obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());

        VkBasicPushConstantData push{
            .offset = obj.transform2d.transform,
            .color = obj.color
        };

        push.transform = obj.transform2d.mat2();

        vkCmdPushConstants(
            commandBuffer,
            pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(VkBasicPushConstantData),
            &push
        );

        obj.model->bind(commandBuffer);
        obj.model->draw(commandBuffer);
    }
}
