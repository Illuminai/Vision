#ifndef VISION_WINDOW_H
#define VISION_WINDOW_H


#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include <GLFW/glfw3.h>
#include <memory>
#include "vulkan/core/Context.h"
#include "vulkan/core/Swapchain.h"
#include "vulkan/core/InFlightFrames.h"
#include "windowing/VulkanWindow.h"

namespace windowing {
    class ImGuiWindow;
}

class windowing::ImGuiWindow : public VulkanWindow {
public:
    ImGuiWindow();

    ~ImGuiWindow();

    virtual void onImGuiFrameRender() = 0;

    void renderImGuiFrame(vulkan::SyncObject syncObject, uint32_t imageIndex, ImDrawData *draw_data);

    void onRender(vulkan::SyncObject syncObject, uint32_t imageIndex) override;

    void onSwapchainRebuild() override;

private:
    ImGuiContext *imguiContext;

    std::vector<VkFramebuffer> framebuffers;

    VkDescriptorPool descriptorPool{VK_NULL_HANDLE};

    VkRenderPass renderPass{VK_NULL_HANDLE};

    void createRenderPass();

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
        static_cast<VulkanWindow *>(glfwGetWindowUserPointer(window))->renderWindow();
    }

};


#endif //VISION_WINDOW_H
