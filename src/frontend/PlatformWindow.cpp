#include "frontend/PlatformWindow.h"


void PlatformWindow::initWindow() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW!");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);

    if (!glfwVulkanSupported()) {
        throw std::runtime_error("Vulkan not supported!");
    }

    vkApplication = VulkanApplication{window};
    vkApplication.initVulkan();
}


void PlatformWindow::loop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        vkApplication.draw();
    }
}

void PlatformWindow::destroyWindow() {
    vkApplication.destroyVulkan();
    glfwDestroyWindow(window);
    glfwTerminate();
}
