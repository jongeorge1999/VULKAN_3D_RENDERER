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
    glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .02f}; // w is intensity
    glm::vec3 lightPosition{-1.f};
    alignas(16) glm::vec4 lightColor{1.f}; // w is light intensity
    // alignas(16) glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
};

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
                camera,
                globalDescriptorSets[frameIndex],
                objects
            };

            //update
            globalUbo ubo{};
            ubo.projectionView = camera.getProjection() * camera.getView();
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            //render
            renderer.beginSwapChainRenderPass(commandBuffer);
            renderSystem.renderObjects(frameInfo);
            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(device.device());
}

void App::loadObjects() {
    std::shared_ptr<Model> stormtrooper = Model::createModelFromFile(device, "../models/stormtrooper.obj");
    std::shared_ptr<Model> vase = Model::createModelFromFile(device, "../models/smooth_vase.obj");
    std::shared_ptr<Model> colored_cube = Model::createModelFromFile(device, "../models/colored_cube.obj");
    std::shared_ptr<Model> floor = Model::createModelFromFile(device, "../models/quad.obj");

    auto floor_obj = Object::createObject();
    floor_obj.model = floor;
    floor_obj.transform.translation = {0.f, 0.5f, 2.5f};
    floor_obj.transform.scale = glm::vec3(3.f, 1.f, 3.f);
    objects.emplace(floor_obj.getId(), std::move(floor_obj));

    auto stormtrooper_obj = Object::createObject();
    stormtrooper_obj.model = stormtrooper;
    stormtrooper_obj.transform.translation = {.0f, .5f, 2.5f};
    stormtrooper_obj.transform.scale = glm::vec3(1.0f);
    stormtrooper_obj.shouldRotateY = true;
    objects.emplace(stormtrooper_obj.getId(), std::move(stormtrooper_obj));

    auto vase_obj = Object::createObject();
    vase_obj.model = vase;
    vase_obj.transform.translation = {-2.0f, .5f, 2.5f};
    vase_obj.transform.scale = glm::vec3(4.0f);
    objects.emplace(vase_obj.getId(), std::move(vase_obj));

    auto colored_cube_obj = Object::createObject();
    colored_cube_obj.model = colored_cube;
    colored_cube_obj.transform.translation = {2.2f, 0.0f, 2.5f};
    colored_cube_obj.transform.scale = glm::vec3(0.5f);
    objects.emplace(colored_cube_obj.getId(), std::move(colored_cube_obj));
}