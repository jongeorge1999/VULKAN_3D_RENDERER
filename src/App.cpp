#include "App.hpp"
#include "Camera.hpp"
#include "KeyboardMoveController.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/PointLightSystem.hpp"
#include "Buffer.hpp"


#include <stdexcept>
#include <cassert>
#include <array>
#include <chrono>
#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#define MAX_FRAME_TIME 16.f

App::App() {
    globalPool = DescriptorPool::Builder(device)
        .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();
    loadObjects();
}

App::~App() {}

void App::run() {
    std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++) {
        uboBuffers[i] = std::make_unique<Buffer>(
            device,
            sizeof(globalUbo),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        );
        uboBuffers[i]->map();
    }

    auto globalSetLayout = DescriptorSetLayout::Builder(device)
        .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
        .build();
    
    std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < globalDescriptorSets.size(); i++) {
       auto bufferInfo = uboBuffers[i]->descriptorInfo();
       DescriptorWriter(*globalSetLayout, *globalPool)
        .writeBuffer(0, &bufferInfo)
        .build(globalDescriptorSets[i]);
    }

    RenderSystem renderSystem{device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
    PointLightSystem pointLightSystem{device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
    Camera camera{};
    // camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
    camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

    auto viewerObject = Object::createObject();
    KeyboardMoveController cameraController{};
    KeyboardMoveController settingsController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while(!window.shouldClose()) {
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        frameTime = glm::min(frameTime, MAX_FRAME_TIME);

        cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        settingsController.settings(window.getGLFWwindow(), frameTime, viewerObject);

        float aspect = renderer.getAspectRatio();
        // camera.setOrthographicProjection(-aspect,aspect,-1,1,-1,1);
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 30.f);

        if(auto commandBuffer = renderer.beginFrame()) {
            int frameIndex = renderer.getFrameIndex();
            FrameInfo frameInfo {
                frameIndex,
                frameTime,
                commandBuffer,
                camera,
                globalDescriptorSets[frameIndex],
                objects
            };

            //update
            globalUbo ubo{};
            ubo.useSpec = viewerObject.useSpec;
            ubo.projection = camera.getProjection();
            ubo.view = camera.getView();
            ubo.inverseView = camera.getInverseView();
            pointLightSystem.update(frameInfo, ubo);
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            //render
            renderer.beginSwapChainRenderPass(commandBuffer);
            renderSystem.renderObjects(frameInfo);
            pointLightSystem.render(frameInfo);
            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(device.device());
}

void App::loadObjects() {
    std::shared_ptr<Model> floor = Model::createModelFromFile(device, "../models/quad.obj");
    auto floor_obj = Object::createObject();
    floor_obj.model = floor;
    floor_obj.transform.translation = {0.f, 0.5f, 0.f};
    floor_obj.transform.scale = glm::vec3(3.f, 1.f, 3.f);
    objects.emplace(floor_obj.getId(), std::move(floor_obj));

    std::shared_ptr<Model> stormtrooper = Model::createModelFromFile(device, "../models/stormtrooper.obj");
    auto stormtrooper_obj = Object::createObject();
    stormtrooper_obj.model = stormtrooper;
    stormtrooper_obj.transform.translation = {.0f, .5f, 0.f};
    stormtrooper_obj.transform.scale = glm::vec3(1.0f);
    stormtrooper_obj.shouldRotateY = true;
    objects.emplace(stormtrooper_obj.getId(), std::move(stormtrooper_obj));

    std::shared_ptr<Model> vase = Model::createModelFromFile(device, "../models/smooth_vase.obj");
    auto vase_obj = Object::createObject();
    vase_obj.model = vase;
    vase_obj.transform.translation = {-2.0f, .5f, 0.f};
    vase_obj.transform.scale = glm::vec3(4.0f);
    objects.emplace(vase_obj.getId(), std::move(vase_obj));

    std::shared_ptr<Model> colored_cube = Model::createModelFromFile(device, "../models/colored_cube.obj");
    auto colored_cube_obj = Object::createObject();
    colored_cube_obj.model = colored_cube;
    colored_cube_obj.transform.translation = {2.2f, 0.0f, 0.f};
    colored_cube_obj.transform.scale = glm::vec3(0.5f);
    objects.emplace(colored_cube_obj.getId(), std::move(colored_cube_obj));

    std::vector<glm::vec3> lightColors{
        {1.f, .1f, .1f},
        {.1f, .1f, 1.f},
        {.1f, 1.f, .1f},
        {1.f, 1.f, .1f},
        {.1f, 1.f, 1.f},
        {1.f, 1.f, 1.f} 
    };

    for(int i = 0; i < lightColors.size(); i++) {
        auto pLight = Object::makePointLight(0.7f);
        pLight.color = lightColors[i];
        auto rotateLight = glm::rotate(glm::mat4(1.f), (i * glm::two_pi<float>()) / lightColors.size(), {0.f, -1.f, 0.f});
        pLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
        objects.emplace(pLight.getId(), std::move(pLight));
    }
}