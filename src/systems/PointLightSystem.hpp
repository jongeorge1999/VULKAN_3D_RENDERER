#pragma once

#include "../Pipeline.hpp"
#include "../Object.hpp"
#include "../Device.hpp"
#include "../Camera.hpp"
#include "../FrameInfo.hpp"

#include <memory>
#include <vector>

class PointLightSystem{
    public:

        PointLightSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~PointLightSystem();

        PointLightSystem(const PointLightSystem&) = delete;
        PointLightSystem& operator=(const PointLightSystem &) = delete;

        void render(FrameInfo& frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        Device& device;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
};