#include "vulkan/core/Context.h"

#include <utility>

namespace vulkan {

    Context::Context(const std::vector<std::tuple<const char *, bool>> &instanceExtensions,
                     const std::vector<const char *> &validationLayers,
                     const std::vector<std::tuple<const char *, bool>> &deviceExtensions, GLFWwindow *window) {

        sharedContext = std::make_shared<SharedContext>(instanceExtensions, validationLayers, deviceExtensions, window);

        auto queueFamily = sharedContext->getDevice().findQueueFamily(VK_QUEUE_GRAPHICS_BIT);
        if (queueFamily.has_value()) {
            generalCommandPool = CommandPool{sharedContext->getDevice().getVkDevice(),
                                             queueFamily->index};
            transientCommandPool = CommandPool{sharedContext->getDevice().getVkDevice(),
                                               queueFamily->index, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT};
        }
    }

    Context::Context(std::shared_ptr<SharedContext> sharedContext) : sharedContext(std::move(sharedContext)) {
        auto queueFamily = sharedContext->getDevice().findQueueFamily(VK_QUEUE_GRAPHICS_BIT);
        if (queueFamily.has_value()) {
            generalCommandPool = CommandPool{sharedContext->getDevice().getVkDevice(),
                                             queueFamily->index};
            transientCommandPool = CommandPool{sharedContext->getDevice().getVkDevice(),
                                               queueFamily->index, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT};
        }

    }

    Context::~Context() {
        vkDestroyCommandPool(sharedContext->getDevice().getVkDevice(), generalCommandPool->getVkCommandPool(), nullptr);
        vkDestroyCommandPool(sharedContext->getDevice().getVkDevice(), transientCommandPool->getVkCommandPool(),
                             nullptr);
    }

    std::shared_ptr<SharedContext> Context::getSharedContext() {
        return sharedContext;
    }

    VkCommandBuffer Context::beginTransientExecution() {
        VkCommandBufferAllocateInfo allocationInfo{};
        allocationInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocationInfo.commandPool = transientCommandPool->getVkCommandPool();
        allocationInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocationInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;

        checkError(vkAllocateCommandBuffers(sharedContext->getDevice().getVkDevice(), &allocationInfo, &commandBuffer),
                   "Transient command buffer allocation");

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        checkError(vkBeginCommandBuffer(commandBuffer, &beginInfo), "Transient command buffer begin");
        return commandBuffer;
    }

    void Context::endTransientExecution(VkCommandBuffer commandBuffer) {
        checkError(vkEndCommandBuffer(commandBuffer), "Transient command buffer end");

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        checkError(vkQueueSubmit(sharedContext->getDevice().getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE),
                   "Transient queue submit");

        checkError(vkQueueWaitIdle(sharedContext->getDevice().getGraphicsQueue()), "Transient queue wait idle");

        vkFreeCommandBuffers(sharedContext->getDevice().getVkDevice(), transientCommandPool->getVkCommandPool(), 1,
                             &commandBuffer);
    }

    Instance Context::getInstance() {
        return sharedContext->getInstance();
    }

    std::optional<Surface> Context::getSurface() {
        return sharedContext->getSurface();
    }

    Device Context::getDevice() {
        return sharedContext->getDevice();
    }

}