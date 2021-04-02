#ifndef VISION_DEVICE_H
#define VISION_DEVICE_H

#include "vulkan/core/Instance.h"
#include "vulkan/core/Surface.h"
#include "vulkan/core/PhysicalDevice.h"

namespace vulkan {
    class Device;

    class QueueFamily;
}

class vulkan::Device {
public:
    Device(VkInstance instance, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
           const std::vector<std::tuple<const char *, bool>> &requiredExtensions);

    void destroy();

    VkPhysicalDevice getPhysicalDevice();

    VkDevice getVkDevice();

private:
    VkDevice vkDevice{VK_NULL_HANDLE};

    VkPhysicalDevice vkPhysicalDevice;

    std::vector<QueueFamily> queueFamilies;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue computeQueue;
    VkQueue transferQueue;

    static std::vector<QueueFamily> findQueueFamilies(VkPhysicalDevice physicalDevice);

};


struct vulkan::QueueFamily {
    uint32_t index;
    VkQueueFamilyProperties properties;
};


#endif //VISION_DEVICE_H
