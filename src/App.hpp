#pragma once

#include "Window.hpp"
#include "Pipeline.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include <memory>
#include <vector>

class App{
    private:
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        Window window{WIDTH, HEIGHT, "VULKAN 3D"};
        Device device{window};
        SwapChain swapChain{device, window.getExtent()};
        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;

    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        App();
        ~App();

        App(const App&) = delete;
        App& operator=(const App &) = delete;

        void run();
};