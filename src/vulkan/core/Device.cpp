#include "vulkan/core/Device.h"

#include "vulkan/debug/ErrorCheck.h"

namespace vulkan {

    Device::Device(VkPhysicalDevice physicalDevice,
                   VkSurfaceKHR surface,
                   const std::vector<std::tuple<const char *, bool>> &requiredExtensions) : vkPhysicalDevice(
            physicalDevice) {

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

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.enabledLayerCount = 0;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        checkError(vkCreateDevice(physicalDevice, &createInfo, nullptr, &vkDevice), "Device creation");

        for (auto index : queueFamilies) {
            if (!std::get<0>(graphicsQueue) && index.properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                vkGetDeviceQueue(vkDevice, index.index, 0, &std::get<0>(graphicsQueue));
                std::get<1>(graphicsQueue) = index.index;
            } else if (!std::get<0>(computeQueue) && index.properties.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                vkGetDeviceQueue(vkDevice, index.index, 0, &std::get<0>(computeQueue));
                std::get<1>(computeQueue) = index.index;
            } else if (!std::get<0>(transferQueue) && index.properties.queueFlags & VK_QUEUE_TRANSFER_BIT) {
                vkGetDeviceQueue(vkDevice, index.index, 0, &std::get<0>(transferQueue));
                std::get<1>(transferQueue) = index.index;
            }
        }
        for (auto index : queueFamilies) {
            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index.index, surface, &presentSupport);
            if (presentSupport) {
                vkGetDeviceQueue(vkDevice, index.index, 0, &std::get<0>(presentQueue));
                std::get<1>(presentQueue) = index.index;
                break;
            }
        }

    }


    std::optional<QueueFamily> Device::findQueueFamily(VkQueueFlagBits flags) {
        for (auto queueFamily : queueFamilies) {
            if (queueFamily.properties.queueFlags & flags) {
                return queueFamily;
            }
        }
        return {};
    }

    VkPhysicalDevice Device::getVkPhysicalDevice() {
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

    VkQueue Device::getGraphicsQueue() {
        return std::get<0>(graphicsQueue);
    }

    uint32_t Device::getGraphicsQueueFamily() {
        return std::get<1>(graphicsQueue);
    }

    VkQueue Device::getPresentQueue() {
        return std::get<0>(presentQueue);
    }

    uint32_t Device::getPresentQueueFamily() {
        return std::get<1>(presentQueue);
    }

    VkQueue Device::getComputeQueue() {
        return std::get<0>(computeQueue);
    }

    uint32_t Device::getComputeQueueFamily() {
        return std::get<1>(computeQueue);
    }

    VkQueue Device::getTransferQueue() {
        return std::get<0>(transferQueue);
    }

    uint32_t Device::getTransferQueueFamily() {
        return std::get<1>(transferQueue);
    }

    std::vector<QueueFamily> Device::getQueueFamilies() {
        return queueFamilies;
    }

}