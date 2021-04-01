#include "vulkan/core/Surface.h"
#include <stdexcept>

namespace vulkan {

    Surface::Surface(const std::shared_ptr<Instance>& instance, GLFWwindow *window) : instance(instance) {
        if (glfwCreateWindowSurface(instance->getVkInstance(), window, nullptr, &surface)) {
            throw std::runtime_error("Failed to create window surface");
        }
    }

    Surface::~Surface() {
        vkDestroySurfaceKHR(instance->getVkInstance(), surface, nullptr);
    }

    VkSurfaceKHR Surface::getVkSurface() {
        return surface;
    }

}