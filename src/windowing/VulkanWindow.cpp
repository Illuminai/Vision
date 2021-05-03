#include "windowing/VulkanWindow.h"


namespace windowing {


    VulkanWindow::VulkanWindow() : BaseWindow(getRequiredWindowHints()) {
        if (!glfwVulkanSupported()) {
            throw std::runtime_error("Vulkan not supported!");
        }

        std::vector<std::tuple<const char *, bool>> instanceExtensions = {
                //std::make_tuple("TESTTEST", false), Extension not optional and not available. will crash.
                std::make_tuple("OOF", true) // Extension optional
        };

        uint32_t extensions_count = 0;
        const char **extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
        std::vector<const char *> glfwExtensions(extensions, extensions + extensions_count);
        for (auto ex : glfwExtensions) {
            instanceExtensions.emplace_back(ex, false);
        }

        const std::vector<const char *> validationLayers = {
                "VK_LAYER_KHRONOS_validation",
                "VK_LAYER_LUNARG_standard_validation"
        };

        const std::vector<std::tuple<const char *, bool>> deviceExtensions = {
                std::make_tuple(VK_KHR_SWAPCHAIN_EXTENSION_NAME, false),
                //std::make_tuple("TESTTEST", false), Extension not optional and not available. will crash.
                std::make_tuple("OOF", true) // Extension optional
        };

        context = std::make_shared<vulkan::Context>(instanceExtensions,
                                                    validationLayers,
                                                    deviceExtensions,
                                                    window);

        int width, height;
        glfwGetWindowSize(window, &width, &height);
        uint32_t windowSize[2] = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        swapchain = std::make_unique<vulkan::Swapchain>(context, windowSize);

        inFlightFrames = std::make_unique<vulkan::InFlightFrames>(context, 2);

        imagesInFlight.resize(swapchain->imageCount, VK_NULL_HANDLE);

        for (int i = 0; i < swapchain->imageCount; ++i) {
            commandPools.emplace_back(vulkan::CommandPool{context->getDevice().getVkDevice(),
                                                          context->getDevice().getGraphicsQueueFamily()});
        }

        commandBuffers.resize(swapchain->imageCount);
        for (int i = 0; i < swapchain->imageCount; ++i) {
            VkCommandBufferAllocateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            info.commandPool = commandPools[i].getVkCommandPool();
            info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            info.commandBufferCount = 1;
            vulkan::checkError(vkAllocateCommandBuffers(context->getDevice().getVkDevice(),
                                                        &info, &commandBuffers[i]),
                               "Command buffer allocation");
        }

        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int, int) {
            ((VulkanWindow *) glfwGetWindowUserPointer(window))->onWindowRender();
        });
    }

    VulkanWindow::~VulkanWindow() {
        vkDeviceWaitIdle(context->getDevice().getVkDevice());

        for (auto commandPool : commandPools) {
            commandPool.destroy(context->getDevice().getVkDevice());
        }

        glfwDestroyWindow(window);
    }

    std::optional<uint32_t> VulkanWindow::acquireNextImage(vulkan::SyncObject syncObject) {
        vkWaitForFences(context->getDevice().getVkDevice(), 1, &syncObject.fence, VK_TRUE, UINT64_MAX);
        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(context->getDevice().getVkDevice(), swapchain->swapchain,
                                                UINT64_MAX, syncObject.imageAvailableSemaphore, VK_NULL_HANDLE,
                                                &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            rebuildSwapchain = true;
            return std::nullopt;
        } else if (result != VK_SUCCESS) {
            vulkan::checkError(result, "Swap chain image acquisition");
        }

        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(context->getDevice().getVkDevice(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        imagesInFlight[imageIndex] = syncObject.fence;
        return imageIndex;
    }

    void VulkanWindow::presentFrame(vulkan::SyncObject syncObject, uint32_t imageIndex) {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        VkSemaphore signalSemaphores[] = {syncObject.renderFinishedSemaphore};
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapchain->swapchain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        VkResult result = vkQueuePresentKHR(context->getDevice().getPresentQueue(), &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || rebuildSwapchain) {
            rebuildSwapchain = false;
        } else if (result != VK_SUCCESS) {
            vulkan::checkError(result, "Swap chain image presentation");
        }
    }

    void VulkanWindow::onWindowRender() {
        if (rebuildSwapchain) {
            recreateSwapchain();
            return;
        }

        auto syncObject = inFlightFrames->nextSyncObject();
        std::optional<uint32_t> imageIndex = acquireNextImage(syncObject);
        if (imageIndex.has_value()) {
            onRender(syncObject, imageIndex.value());
            presentFrame(syncObject, imageIndex.value());
        }
    }

    void VulkanWindow::recreateSwapchain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        if (width != 0 && height != 0) {
            vkDeviceWaitIdle(context->getDevice().getVkDevice());

            uint32_t dim[2] = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
            swapchain->rebuild(dim);

            imagesInFlight.resize(swapchain->imageCount, VK_NULL_HANDLE);

            onSwapchainRebuild();

            rebuildSwapchain = false;
        }
    }

    std::vector<std::tuple<int, int>> VulkanWindow::getRequiredWindowHints() {
        std::vector<std::tuple<int, int>> hints;
        hints.emplace_back(GLFW_CLIENT_API, GLFW_NO_API);
        return hints;
    }

}