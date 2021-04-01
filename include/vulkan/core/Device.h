#ifndef VISION_DEVICE_H
#define VISION_DEVICE_H

#include "vulkan/core/Instance.h"
#include "vulkan/core/Surface.h"
#include "vulkan/core/PhysicalDevice.h"

namespace vulkan {
    class Device;
}

//TODO
class vulkan::Device {
public:
    Device(VkInstance instance, VkPhysicalDevice physicalDevice);

    VkPhysicalDevice getPhysicalDevice();

    VkDevice getVkDevice();

private:
    VkDevice vkDevice;

    VkPhysicalDevice physicalDevice;


    //static QueueFamilyIndices
    //findQueueFamilies(VkInstance &instance, Surface &surface, VkPhysicalDevice physicalDevice);

};


#endif //VISION_DEVICE_H
