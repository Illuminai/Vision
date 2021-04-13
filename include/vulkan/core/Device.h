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
    Device(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
           const std::vector<std::tuple<const char *, bool>> &requiredExtensions);

    void destroy();

    std::optional<QueueFamily> findQueueFamily(VkQueueFlagBits flags);

    VkPhysicalDevice getVkPhysicalDevice();

    VkDevice getVkDevice();

    VkQueue getGraphicsQueue();

    uint32_t getGraphicsQueueFamily();

    VkQueue getPresentQueue();

    uint32_t getPresentQueueFamily();

    VkQueue getComputeQueue();

    uint32_t getComputeQueueFamily();

    VkQueue getTransferQueue();

    uint32_t getTransferQueueFamily();

    std::vector<QueueFamily> getQueueFamilies();

private:
    VkDevice vkDevice{VK_NULL_HANDLE};

    VkPhysicalDevice vkPhysicalDevice{VK_NULL_HANDLE};

    std::vector<QueueFamily> queueFamilies;

    std::tuple<VkQueue, uint32_t> graphicsQueue{VK_NULL_HANDLE, 0};
    std::tuple<VkQueue, uint32_t> presentQueue{VK_NULL_HANDLE, 0};
    std::tuple<VkQueue, uint32_t> computeQueue{VK_NULL_HANDLE, 0};
    std::tuple<VkQueue, uint32_t> transferQueue{VK_NULL_HANDLE, 0};

    static std::vector<QueueFamily> findQueueFamilies(VkPhysicalDevice physicalDevice);

};


struct vulkan::QueueFamily {
    uint32_t index;
    VkQueueFamilyProperties properties;
};


#endif //VISION_DEVICE_H
