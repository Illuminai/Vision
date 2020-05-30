#ifndef VISION_QUEUEFAMILYINDICES_H
#define VISION_QUEUEFAMILYINDICES_H

#include "vulkan/vulkan.hpp"


class QueueFamilyIndices {
public:
    int presentFamily = -1;
    int computeFamily = -1;

    bool isComplete();
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);


#endif //VISION_QUEUEFAMILYINDICES_H
