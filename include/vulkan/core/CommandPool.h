#ifndef VISION_COMMANDPOOL_H
#define VISION_COMMANDPOOL_H

#include <vulkan/vulkan.hpp>

namespace vulkan {
    class CommandPool;
}

class vulkan::CommandPool {
public:
    CommandPool(VkDevice device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = 0);

    void destroy(VkDevice device);

    VkCommandPool getVkCommandPool();

private:
    VkCommandPool vkCommandPool{VK_NULL_HANDLE};

};


#endif //VISION_COMMANDPOOL_H
