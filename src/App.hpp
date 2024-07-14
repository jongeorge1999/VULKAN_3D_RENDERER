#pragma once

#include "Window.hpp"
#include "Object.hpp"
#include "Device.hpp"
#include "Renderer.hpp"
#include "Descriptors.hpp"
#include <memory>
#include <vector>

class App{
    public:
        static constexpr int WIDTH = 1600;
        static constexpr int HEIGHT = 1200;

        App();
        ~App();

        App(const App&) = delete;
        App& operator=(const App &) = delete;

        void run();

    private:
        void loadObjects();

        Window window{WIDTH, HEIGHT, "VULKAN_3D_RENDERER"};
        Device device{window};
        Renderer renderer{window, device};

        std::unique_ptr<DescriptorPool> globalPool{};
        Object::Map objects;
};