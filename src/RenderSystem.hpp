#pragma once

#include "Pipeline.hpp"
#include "Object.hpp"
#include "Device.hpp"
#include "Camera.hpp"

#include <memory>
#include <vector>

class RenderSystem{
    public:

        RenderSystem(Device& device, VkRenderPass renderPass);
        ~RenderSystem();

        RenderSystem(const RenderSystem&) = delete;
        RenderSystem& operator=(const RenderSystem &) = delete;

        void renderObjects(VkCommandBuffer commandBuffer, std::vector<Object>& objects, const Camera& camera);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        Device& device;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
};