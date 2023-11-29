#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "Device.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <vector>

struct VKPipelineConfigurationInformation {
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class VKPipeline {
    public:
    VKPipeline(
        VKDevice& device,
        const std::string& vertShaderPath, const std::string& fragShaderPath,
        const VKPipelineConfigurationInformation& configInfo
    ) :
    info(configInfo),
    device(device)
    {
        createVkPipeline(vertShaderPath, fragShaderPath);
    };

    ~VKPipeline();

    VKPipeline(const VKPipeline&) = delete;
    VKPipeline &operator=(const VKPipeline&) = delete;

    static void defaultPipelineConfigInfo(VKPipelineConfigurationInformation& configInfo);
    void bind(VkCommandBuffer commandBuffer);
    private:
    const VKPipelineConfigurationInformation& info;
    VKDevice& device;
    VkPipeline pipeline;
    VkShaderModule vertShaderModule;  
    VkShaderModule fragShaderModule;  

    void createVkPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath);
    void createShaderModule(const std::vector<char>& code, VkShaderModule *shaderMod);
};

#endif // PIPELINE_H_