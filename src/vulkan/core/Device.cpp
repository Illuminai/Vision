#include "vulkan/core/Device.h"

namespace vulkan {


    Device::Device(VkInstance instance, VkPhysicalDevice physicalDevice) : physicalDevice(physicalDevice) {

    }

    VkPhysicalDevice Device::getPhysicalDevice() {
        return physicalDevice;
    }

    VkDevice Device::getVkDevice() {
        return vkDevice;
    }


}