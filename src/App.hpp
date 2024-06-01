#pragma once

#include "Window.hpp"
#include "Pipeline.hpp"
#include "Object.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include <memory>
#include <vector>

class App{
    private:
        void loadObjects();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);
        void renderObjects(VkCommandBuffer commandBuffer);

        Window window{WIDTH, HEIGHT, "VULKAN 3D"};
        Device device{window};
        std::unique_ptr<SwapChain> swapChain;
        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<Object> objects;

    public:
        static constexpr int WIDTH = 1600;
        static constexpr int HEIGHT = 1200;

        App();
        ~App();

        App(const App&) = delete;
        App& operator=(const App &) = delete;

        void run();
};