#include "vulkan/core/CommandPool.h"

#include "vulkan/debug/ErrorCheck.h"

namespace vulkan {

    CommandPool::CommandPool(VkDevice device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags) {
        VkCommandPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.queueFamilyIndex = queueFamilyIndex;
        createInfo.flags = createFlags;

        checkError(vkCreateCommandPool(device, &createInfo, nullptr, &vkCommandPool), "Command pool creation");
    }

    void CommandPool::destroy(VkDevice device) {
        if (vkCommandPool) {
            vkDestroyCommandPool(device, vkCommandPool, nullptr);
        }
    }

    VkCommandPool CommandPool::getVkCommandPool() {
        return vkCommandPool;
    }

}