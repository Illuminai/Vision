#ifndef VISION_VULKANWINDOW_H
#define VISION_VULKANWINDOW_H

#include "vulkan/core/Context.h"
#include "vulkan/core/Swapchain.h"
#include "vulkan/core/InFlightFrames.h"

namespace windowing {
    class VulkanWindow;
}

class windowing::VulkanWindow {
public:
    VulkanWindow();

    ~VulkanWindow();

    virtual void onRender(vulkan::SyncObject syncObject, uint32_t imageIndex) = 0;

    virtual void onSwapchainRebuild() = 0;

    virtual void presentFrame(vulkan::SyncObject syncObject, uint32_t imageIndex);

    virtual void recreateSwapchain();

    uint32_t acquireNextImage(vulkan::SyncObject syncObject);

    void loopWindow();

protected:
    //GLFWwindow *window;

    //std::shared_ptr<vulkan::Context> context;

    //std::unique_ptr<vulkan::Swapchain> swapchain;

    //std::vector<VkFramebuffer> framebuffers;

    //std::unique_ptr<vulkan::InFlightFrames> inFlightFrames;

    //std::vector<VkFence> imagesInFlight;

    //bool rebuildSwapchain{false};



};


#endif //VISION_VULKANWINDOW_H
