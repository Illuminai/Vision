#include "vulkan/core/Surface.h"
#include "vulkan/utils/ErrorCheck.h"
#include <stdexcept>

namespace vulkan {

    Surface::Surface(VkInstance instance, GLFWwindow *window) {
        checkError(glfwCreateWindowSurface(instance, window, nullptr, &surface), "Surface creation");
    }

    void Surface::destroy(VkInstance instance) {
        if (surface) {
            vkDestroySurfaceKHR(instance, surface, nullptr);
        }
    }

    VkSurfaceKHR Surface::getVkSurface() {
        return surface;
    }


}