#include "App.hpp"
#include "Camera.hpp"
#include "KeyboardMoveController.hpp"
#include "RenderSystem.hpp"
#include "Buffer.hpp"
#include <stdexcept>
#include <cassert>
#include <array>
#include <chrono>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#define MAX_FRAME_TIME 16.f

struct globalUbo {
    glm::mat4 projectionView{1.f};
    glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
};

App::App() {loadObjects();}

App::~App() {}

void App::run() {

    Buffer globalUboBuffer {
        device,
        sizeof(globalUbo),
        SwapChain::MAX_FRAMES_IN_FLIGHT,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        device.properties.limits.minUniformBufferOffsetAlignment
    };

    globalUboBuffer.map();

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
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 30.f);

        if(auto commandBuffer = renderer.beginFrame()) {
            int frameIndex = renderer.getFrameIndex();
            FrameInfo frameInfo {
                frameIndex,
                frameTime,
                commandBuffer,
                camera
            };

            //update
            globalUbo ubo{};
            ubo.projectionView = camera.getProjection() * camera.getView();
            globalUboBuffer.writeToIndex(&ubo, frameIndex);
            globalUboBuffer.flushIndex(frameIndex);

            //render
            renderer.beginSwapChainRenderPass(commandBuffer);
            renderSystem.renderObjects(frameInfo, objects);
            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(device.device());
}

void App::loadObjects() {
    std::shared_ptr<Model> model = Model::createModelFromFile(device, "../models/stormtrooper.obj");
    std::shared_ptr<Model> model2 = Model::createModelFromFile(device, "../models/smooth_vase.obj");
    std::shared_ptr<Model> model3 = Model::createModelFromFile(device, "../models/colored_cube.obj");

    auto object = Object::createObject();
    object.model = model;
    object.transform.translation = {.0f, .5f, 2.5f};
    object.transform.scale = glm::vec3(1.0f);
    objects.push_back(std::move(object));

    auto object2 = Object::createObject();
    object2.model = model2;
    object2.transform.translation = {-2.0f, .5f, 2.5f};
    object2.transform.scale = glm::vec3(4.0f);
    objects.push_back(std::move(object2));

    auto object3 = Object::createObject();
    object3.model = model3;
    object3.transform.translation = {2.0f, 0.0f, 2.5f};
    object3.transform.scale = glm::vec3(0.5f);
    objects.push_back(std::move(object3));
}