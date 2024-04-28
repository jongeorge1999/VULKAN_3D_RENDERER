#pragma once

#include "window.hpp"

class App{
    private:
        Window window{WIDTH, HEIGHT, "VULKAN 3D"};

    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();
};