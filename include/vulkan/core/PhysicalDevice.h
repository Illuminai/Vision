#ifndef VISION_PHYSICALDEVICE_H
#define VISION_PHYSICALDEVICE_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include "vulkan/core/Instance.h"

namespace vulkan::PhysicalDevice {
    std::vector<VkPhysicalDevice>
    enumerateSupportedDevices(VkInstance instance,
                              const std::vector<std::tuple<const char *, bool>> &requiredExtensions,
                              VkSurfaceKHR surface);

    std::vector<VkPhysicalDevice>
    enumerateSupportedDevices(VkInstance instance,
                              const std::vector<std::tuple<const char *, bool>> &requiredExtensions);

    std::vector<VkPhysicalDevice> enumeratePhysicalDevices(VkInstance instance);

    bool isExtensionAvailable(VkPhysicalDevice &physicalDevice, const char *extensionName);

    std::vector<VkExtensionProperties> getAvailableExtensions(VkPhysicalDevice &physicalDevice);

    std::vector<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice &physicalDevice);
}


#endif //VISION_PHYSICALDEVICE_H
