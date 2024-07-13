#include "App.hpp"
#include "Camera.hpp"
#include "KeyboardMoveController.hpp"
#include "RenderSystem.hpp"
#include <stdexcept>
#include <cassert>
#include <array>
#include <chrono>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#define MAX_FRAME_TIME 16.f

App::App() {
    loadObjects();
}

App::~App() {}

void App::run() {
    RenderSystem renderSystem{device, renderer.getSwapChainRenderPass()};
    Camera camera{};
    // camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
    camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

    auto viewerObject = Object::createObject();
    KeyboardMoveController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while(!window.shouldClose()) {
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        frameTime = glm::min(frameTime, MAX_FRAME_TIME);

        cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = renderer.getAspectRatio();
        // camera.setOrthographicProjection(-aspect,aspect,-1,1,-1,1);
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

        if(auto commandBuffer = renderer.beginFrame()) {
            renderer.beginSwapChainRenderPass(commandBuffer);
            renderSystem.renderObjects(commandBuffer, objects, camera);
            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(device.device());
}

void App::loadObjects() {
    std::shared_ptr<Model> model = Model::createModelFromFile(device, "../models/smooth_vase.obj");

    auto object = Object::createObject();
    object.model = model;
    object.transform.translation = {.0f, .0f, 2.5f};
    object.transform.scale = glm::vec3(3.f);
    objects.push_back(std::move(object));
}