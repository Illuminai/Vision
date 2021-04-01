#include "vulkan/core/PhysicalDevice.h"

namespace vulkan::PhysicalDevice {

    std::vector<VkPhysicalDevice> enumerateSupportedDevices(VkInstance instance,
                                                            const std::vector<std::tuple<const char *, bool>> &requiredExtensions,
                                                            VkSurfaceKHR surface) {
        std::vector<VkPhysicalDevice> devices = enumeratePhysicalDevices(instance);
        std::vector<VkPhysicalDevice> supportedDevices;

        for (auto physicalDevice : devices) {
            for (auto tuple : requiredExtensions) {
                bool available = isExtensionAvailable(physicalDevice, std::get<0>(tuple));
                if (!std::get<1>(tuple) && !available) {
                    continue;
                }
            }

            bool supported = false;
            auto queueFamilies = getQueueFamilyProperties(physicalDevice);

            int index = 0;
            for (const auto &queueFamily : queueFamilies) {
                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface, &presentSupport);
                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupport) {
                    supported = true;
                }
                index++;
            }

            if (supported) {
                supportedDevices.push_back(physicalDevice);
            }
        }

        return supportedDevices;
    }

    std::vector<VkPhysicalDevice>
    enumerateSupportedDevices(VkInstance instance,
                              const std::vector<std::tuple<const char *, bool>> &requiredExtensions) {
        std::vector<VkPhysicalDevice> devices = enumeratePhysicalDevices(instance);
        std::vector<VkPhysicalDevice> supportedDevices;

        for (auto physicalDevice : devices) {
            for (auto tuple : requiredExtensions) {
                bool available = isExtensionAvailable(physicalDevice, std::get<0>(tuple));
                if (!std::get<1>(tuple) && !available) {
                    continue;
                }
            }
            supportedDevices.push_back(physicalDevice);
        }

        return supportedDevices;
    }


    std::vector<VkPhysicalDevice> enumeratePhysicalDevices(VkInstance instance) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        return devices;
    }

    std::vector<VkExtensionProperties> getAvailableExtensions(VkPhysicalDevice &physicalDevice) {
        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());
        return extensions;
    }

    bool isExtensionAvailable(VkPhysicalDevice &physicalDevice, const char *extensionName) {
        auto presentExtensions = getAvailableExtensions(physicalDevice);
        bool found = false;
        for (auto extension : presentExtensions) {
            if (strcmp(extension.extensionName, extensionName) == 0) {
                found = true;
            }
        }
        return found;
    }

    std::vector<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice &physicalDevice) {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        return queueFamilies;
    }

}