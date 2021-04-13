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

namespace windowing {
    class ImGuiWindow;
}

class windowing::ImGuiWindow {
public:
    ImGuiWindow();

    ~ImGuiWindow();

    //TODO
    //ImGui window should be an abstract class. Implement onRenderUI for displaying ImGui UI.
    //virtual void onRenderUI() = 0;

    void renderImGui();

    void loopWindow();

private:
    GLFWwindow *window;

    std::shared_ptr<vulkan::Context> context;

    bool rebuildSwapchain{false};

    ImGuiContext *imguiContext;

    ImGui_ImplVulkanH_Window imguiWindowData;

    VkDescriptorPool descriptorPool{VK_NULL_HANDLE};

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

    void renderFrame(ImDrawData *draw_data);

    void presentFrame();

};


#endif //VISION_WINDOW_H
