#ifndef VISION_VULKANWINDOW_H
#define VISION_VULKANWINDOW_H

#include "vulkan/core/Context.h"
#include "vulkan/core/Swapchain.h"
#include "vulkan/core/InFlightFrames.h"
#include "windowing/BaseWindow.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace windowing {
    class VulkanWindow;
}

class windowing::VulkanWindow : public BaseWindow {
public:
    VulkanWindow();

    ~VulkanWindow();


    // Functions to implement in child class

    virtual void onRender(vulkan::SyncObject syncObject, uint32_t imageIndex) = 0;

    virtual void onSwapchainRebuild() = 0;

    // Internal

    //void renderImGuiFrame(vulkan::SyncObject syncObject, uint32_t imageIndex, ImDrawData* draw_data);


    virtual uint32_t acquireNextImage(vulkan::SyncObject syncObject);

    void presentFrame(vulkan::SyncObject syncObject, uint32_t imageIndex);

    void recreateSwapchain();

    void renderWindow() override;

protected:
    std::shared_ptr<vulkan::Context> context;

    std::unique_ptr<vulkan::Swapchain> swapchain;

    std::vector<vulkan::CommandPool> commandPools;

    std::vector<VkCommandBuffer> commandBuffers;

    std::unique_ptr<vulkan::InFlightFrames> inFlightFrames;

    std::vector<VkFence> imagesInFlight;

    bool rebuildSwapchain{false};


};


#endif //VISION_VULKANWINDOW_H
