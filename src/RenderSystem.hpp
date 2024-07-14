#pragma once

#include "Pipeline.hpp"
#include "Object.hpp"
#include "Device.hpp"
#include "Camera.hpp"
#include "FrameInfo.hpp"

#include <memory>
#include <vector>

class RenderSystem{
    public:

        RenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~RenderSystem();

        RenderSystem(const RenderSystem&) = delete;
        RenderSystem& operator=(const RenderSystem &) = delete;

        void renderObjects(
            FrameInfo& frameInfo,
            std::vector<Object>& objects
        );

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        Device& device;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
};