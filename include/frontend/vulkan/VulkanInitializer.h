#ifndef VISION_VULKANINITIALIZER_H
#define VISION_VULKANINITIALIZER_H

#include "vulkan/vulkan.hpp"

namespace VulkanInitializer {

    inline VkApplicationInfo createApplicationInfo(const char *appName, uint32_t appVersion, uint32_t apiVersion) {
        VkApplicationInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        vk.pApplicationName = appName;
        vk.applicationVersion = appVersion;
        vk.apiVersion = apiVersion;
        return vk;
    }

    inline VkInstanceCreateInfo createInstanceCreateInfo(const VkApplicationInfo appInfo) {
        VkInstanceCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vk.pApplicationInfo = &appInfo;
        return vk;
    }

    inline VkDeviceQueueCreateInfo createDeviceQueueCreateInfo(uint32_t queueFamily,
                                                               const float queuePriority,
                                                               uint32_t queueCount = 1) {
        VkDeviceQueueCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        vk.queueFamilyIndex = queueFamily;
        vk.queueCount = queueCount;
        vk.pQueuePriorities = &queuePriority;
        return vk;
    }

    inline VkDeviceCreateInfo createDeviceCreateInfo() {
        VkDeviceCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        return vk;
    }

    inline VkSwapchainCreateInfoKHR createSwapchainCreateInfoKHR(VkSurfaceKHR surface) {
        VkSwapchainCreateInfoKHR vk{};
        vk.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        vk.surface = surface;
        return vk;
    }

    inline VkImageViewCreateInfo createImageViewCreateInfo(VkImage image, VkImageViewType viewType) {
        VkImageViewCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        vk.image = image;
        vk.viewType = viewType;
        return vk;
    }

    inline VkImageCreateInfo createImageCreateInfo(VkImageType imageType) {
        VkImageCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        vk.imageType = imageType;
        return vk;
    }

    inline VkMemoryAllocateInfo createMemoryAllocateInfo(VkDeviceSize allocationSize, uint32_t memoryTypeIndex) {
        VkMemoryAllocateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        vk.allocationSize = allocationSize;
        vk.memoryTypeIndex = memoryTypeIndex;
        return vk;
    }

    inline VkDescriptorPoolSize createDescriptorPoolSize(VkDescriptorType type, uint32_t descriptionCount) {
        VkDescriptorPoolSize vk{};
        vk.type = type;
        vk.descriptorCount = descriptionCount;
        return vk;
    }

    inline VkDescriptorPoolCreateInfo createDescriptorPoolCreateInfo() {
        VkDescriptorPoolCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        return vk;
    }

    inline VkBufferCreateInfo
    createBufferCreateInfo(VkBufferUsageFlags flags, VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE) {
        VkBufferCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vk.usage = flags;
        vk.sharingMode = sharingMode;
        return vk;
    }

    inline VkDescriptorSetLayoutBinding
    createDescriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t binding,
                                     uint32_t descriptorCount = 1) {
        VkDescriptorSetLayoutBinding vk{};
        vk.descriptorType = type;
        vk.stageFlags = stageFlags;
        vk.descriptorCount = descriptorCount;
        vk.binding = binding;
        return vk;
    }

    inline VkDescriptorSetLayoutCreateInfo createDescriptorSetLayoutCreateInfo() {
        VkDescriptorSetLayoutCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        return vk;
    }

    inline VkPipelineLayoutCreateInfo createPipelineLayoutCreateInfo() {
        VkPipelineLayoutCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        return vk;
    }

    inline VkDescriptorSetAllocateInfo createDescriptorSetAllocateInfo(VkDescriptorPool pool) {
        VkDescriptorSetAllocateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        vk.descriptorPool = pool;
        return vk;
    }

    inline VkDescriptorImageInfo createDescriptorImageInfo(VkImageView imageView, VkImageLayout imageLayout) {
        VkDescriptorImageInfo vk{};
        vk.imageView = imageView;
        vk.imageLayout = imageLayout;
        return vk;
    }

    inline VkDescriptorBufferInfo
    createDescriptorBufferInfo(VkBuffer buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE) {
        VkDescriptorBufferInfo vk{};
        vk.buffer = buffer;
        vk.offset = offset;
        vk.range = range;
        return vk;
    }

    inline VkWriteDescriptorSet createWriteDescriptorSet(VkDescriptorSet set, uint32_t binding, VkDescriptorType type,
                                                         VkDescriptorImageInfo *imageInfo,
                                                         uint32_t descriptorCount = 1) {
        VkWriteDescriptorSet vk{};
        vk.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        vk.dstSet = set;
        vk.dstBinding = binding;
        vk.descriptorType = type;
        vk.descriptorCount = descriptorCount;
        vk.pImageInfo = imageInfo;
        return vk;
    }

    inline VkWriteDescriptorSet createWriteDescriptorSet(VkDescriptorSet set, uint32_t binding, VkDescriptorType type,
                                                         VkDescriptorBufferInfo *bufferInfo,
                                                         uint32_t descriptorCount = 1) {
        VkWriteDescriptorSet vk{};
        vk.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        vk.dstSet = set;
        vk.dstBinding = binding;
        vk.descriptorType = type;
        vk.descriptorCount = descriptorCount;
        vk.pBufferInfo = bufferInfo;
        return vk;
    }

    inline VkShaderModuleCreateInfo createShaderModuleCreateInfo() {
        VkShaderModuleCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        return vk;
    }

    inline VkPipelineShaderStageCreateInfo createPipelineShaderStageCreateInfo() {
        VkPipelineShaderStageCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        return vk;
    }

    inline VkComputePipelineCreateInfo createComputePipelineCreateInfo() {
        VkComputePipelineCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        return vk;
    }

    inline VkCommandPoolCreateInfo createCommandPoolCreateInfo(VkCommandPoolCreateFlags flags) {
        VkCommandPoolCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        vk.flags = flags;
        return vk;
    }

    inline VkCommandBufferAllocateInfo
    createCommandBufferAllocateInfo(VkCommandPool pool, VkCommandBufferLevel level, uint32_t cmdBufferCount = 1) {
        VkCommandBufferAllocateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        vk.commandPool = pool;
        vk.level = level;
        vk.commandBufferCount = cmdBufferCount;
        return vk;
    }


    inline VkCommandBufferBeginInfo createCommandBufferBeginInfo(VkCommandBufferUsageFlags flags) {
        VkCommandBufferBeginInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vk.flags = flags;
        return vk;
    }

    inline VkImageMemoryBarrier
    createImageMemoryBarrier(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) {
        VkImageMemoryBarrier vk{};
        vk.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        vk.image = image;
        vk.oldLayout = oldLayout;
        vk.newLayout = newLayout;
        return vk;
    }

    inline VkFenceCreateInfo createFenceCreateInfo(VkFenceCreateFlags flags) {
        VkFenceCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        vk.flags = flags;
        return vk;
    }

    inline VkSemaphoreCreateInfo createSemaphoreCreateInfo() {
        VkSemaphoreCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        return vk;
    }


}


#endif //VISION_VULKANINITIALIZER_H
