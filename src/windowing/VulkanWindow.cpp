#include "windowing/VulkanWindow.h"


namespace windowing {


    VulkanWindow::VulkanWindow() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        if (!glfwVulkanSupported()) {
            throw std::runtime_error("Vulkan not supported!");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);

        std::vector<std::tuple<const char *, bool>> instanceExtensions = {
                //std::make_tuple("TESTTEST", false), Extension not optional and not available. will crash.
                std::make_tuple("OOF", true) // Extension optional
        };
        const std::vector<const char *> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
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
    }

    VulkanWindow::~VulkanWindow() {
        glfwDestroyWindow(window);
        //glfwTerminate();
    }

    void VulkanWindow::loopWindow() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            if (rebuildSwapchain) {
                onSwapchainRebuild();
                rebuildSwapchain = false;
            }

            auto syncObject = inFlightFrames->nextSyncObject();
            uint32_t imageIndex = acquireNextImage(syncObject);
            onRender(syncObject, imageIndex);
            presentFrame(syncObject, imageIndex);
        }
    }

    uint32_t VulkanWindow::acquireNextImage(vulkan::SyncObject syncObject) {
        vkWaitForFences(context->getDevice().getVkDevice(), 1, &syncObject.fence, VK_TRUE, UINT64_MAX);
        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(context->getDevice().getVkDevice(), swapchain->swapchain,
                                                UINT64_MAX, syncObject.imageAvailableSemaphore, VK_NULL_HANDLE,
                                                &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapchain();
            return -1;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(context->getDevice().getVkDevice(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        imagesInFlight[imageIndex] = syncObject.fence;
        return imageIndex;
    }

    void VulkanWindow::presentFrame(vulkan::SyncObject syncObject, uint32_t imageIndex) {
        /*

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {syncObject.imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = {syncObject.renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(context->getDevice().getVkDevice(), 1, &syncObject.fence);

        if (vkQueueSubmit(context->getDevice().getGraphicsQueue(), 1, &submitInfo, syncObject.fence) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }*/

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
            recreateSwapchain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }

    void VulkanWindow::recreateSwapchain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(context->getDevice().getVkDevice());

        swapchain->rebuild();

        //framebuffers = swapchain->createFramebuffers();

        //createRenderPass();
        //createGraphicsPipeline();
        //createFramebuffers();
        //createCommandBuffers();

        imagesInFlight.resize(swapchain->imageCount, VK_NULL_HANDLE);

    }


}