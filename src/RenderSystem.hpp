#pragma once

#include "Pipeline.hpp"
#include "Object.hpp"
#include "Device.hpp"

#include <memory>
#include <vector>

class RendererSystem{
    public:

        RendererSystem(Device& device, VkRenderPass renderPass);
        ~RendererSystem();

        RendererSystem(const RendererSystem&) = delete;
        RendererSystem& operator=(const RendererSystem &) = delete;

        void renderObjects(VkCommandBuffer commandBuffer, std::vector<Object>& objects);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        Device& device;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
};