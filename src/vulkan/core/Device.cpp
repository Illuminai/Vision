#include "vulkan/core/Device.h"

#include "vulkan/utils/ErrorCheck.h"

namespace vulkan {

    Device::Device(VkInstance instance,
                   VkPhysicalDevice physicalDevice,
                   VkSurfaceKHR surface,
                   const std::vector<std::tuple<const char *, bool>> &requiredExtensions) {
        // ARE EXTENSIONS SUPPORTED?
        std::vector<const char *> extensions;
        for (auto tuple : requiredExtensions) {
            bool available = vulkan::PhysicalDevice::isExtensionAvailable(physicalDevice, std::get<0>(tuple));
            if (!std::get<1>(tuple) && !available) {
                std::string buffer("Device extension ");
                buffer.append(std::get<0>(tuple));
                buffer.append(" not present");
                throw std::runtime_error(buffer);
            }
            if (available) {
                extensions.push_back(std::get<0>(tuple));
            }
        }


        // INIT QUEUE FAMILIES
        queueFamilies = findQueueFamilies(physicalDevice);
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        float queuePriority = 1.0f;
        for (auto index : queueFamilies) {
            uint32_t queueFamilyIndex = index.index;

            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            queueCreateInfos.push_back(queueCreateInfo);
        }

        //CREATE DEVICE
        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.fillModeNonSolid = VK_TRUE;
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkPhysicalDeviceDescriptorIndexingFeaturesEXT indexingFeatures = {};
        indexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;
        indexingFeatures.runtimeDescriptorArray = VK_TRUE;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pNext = &indexingFeatures;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledLayerCount = 0;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        checkError(vkCreateDevice(physicalDevice, &createInfo, nullptr, &vkDevice), "Device creation");

        for (auto index : queueFamilies) {
            if (!graphicsQueue && index.properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                vkGetDeviceQueue(vkDevice, index.index, 0, &graphicsQueue);
            } else if (!computeQueue && index.properties.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                vkGetDeviceQueue(vkDevice, index.index, 0, &computeQueue);
            } else if (!transferQueue && index.properties.queueFlags & VK_QUEUE_TRANSFER_BIT) {
                vkGetDeviceQueue(vkDevice, index.index, 0, &transferQueue);
            }
        }
        for (auto index : queueFamilies) {
            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index.index, surface, &presentSupport);
            if (presentSupport) {
                vkGetDeviceQueue(vkDevice, index.index, 0, &presentQueue);
                break;
            }
        }

    }

    VkPhysicalDevice Device::getPhysicalDevice() {
        return vkPhysicalDevice;
    }

    VkDevice Device::getVkDevice() {
        return vkDevice;
    }

    void Device::destroy() {
        if (vkDevice) {
            vkDestroyDevice(vkDevice, nullptr);
        }
    }

    std::vector<QueueFamily> Device::findQueueFamilies(VkPhysicalDevice physicalDevice) {
        std::vector<QueueFamily> indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        uint32_t i = 0;
        for (const auto &queueFamily : queueFamilies) {
            QueueFamily index{i, queueFamily};
            indices.push_back(index);
            i++;
        }

        return indices;
    }


}