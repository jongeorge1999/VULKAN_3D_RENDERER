#include "App.hpp"
#include "RenderSystem.hpp"
#include <stdexcept>
#include <cassert>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

App::App() {
    loadObjects();
}

App::~App() {}

void App::run() {
    RendererSystem renderSystem{device, renderer.getSwapChainRenderPass()};

    while(!window.shouldClose()) {
        glfwPollEvents();

        if(auto commandBuffer = renderer.beginFrame()) {
            renderer.beginSwapChainRenderPass(commandBuffer);
            renderSystem.renderObjects(commandBuffer, objects);
            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(device.device());
}

void App::loadObjects() {
    std::vector<Model::Vertex> vertices { //the first group is position coordinates and the second group(vector3) is the color information for that vertex
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    auto model = std::make_shared<Model>(device, vertices);

    auto triangle = Object::createObject();
    triangle.model = model;
    triangle.color = {.9f,.9f, .1f};
    triangle.transform2d.translation.x = .2f;
    triangle.transform2d.scale = {2.f, .5f};
    triangle.transform2d.rotation = .25f * glm::two_pi<float>();

    objects.push_back(std::move(triangle));
}