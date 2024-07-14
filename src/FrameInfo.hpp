#pragma once

#include "Camera.hpp"
#include "Object.hpp"

#include <vulkan/vulkan.h>

struct FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    Camera& camera;
    VkDescriptorSet globalDescriptorSet;
    Object::Map& objects;
};