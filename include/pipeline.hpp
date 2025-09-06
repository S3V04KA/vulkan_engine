#pragma once

#include "device.hpp"

#include <string>
#include <vector>

namespace hex
{

    struct PipelineConfigInfo
    {
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo;
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

    class Pipeline
    {
    public:
        Pipeline(Device &device, const std::string &vertShaderName, const std::string &fragShaderName, const PipelineConfigInfo &config);
        ~Pipeline();

        Pipeline(const Pipeline &) = delete;
        Pipeline &operator=(const Pipeline &) = delete;

        void bind(VkCommandBuffer commandBuffer);
        static void defaultPipelineConigInfo(PipelineConfigInfo &config);

    private:
        static std::vector<char> readShader(const std::string &shaderName);

        void createGraphicsPipeline(const std::string &vertShaderName, const std::string &fragShaderName, const PipelineConfigInfo &config);

        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        Device &device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}