#include "vulkan/core/SharedContext.h"

#include <utility>

namespace vulkan {

    SharedContext::SharedContext(const std::vector<std::tuple<const char *, bool>> &instanceExtensions,
                                 const std::vector<const char *> &validationLayers,
                                 std::vector<std::tuple<const char *, bool>> deviceExtensions,
                                 GLFWwindow *window) :
            instance(createInstance(instanceExtensions, validationLayers, true)),
            surface(createSurface(instance, window)),
            device(createDevice(instance, surface.value(), std::move(deviceExtensions))) {}

    SharedContext::~SharedContext() {
        device.destroy();
        if (surface.has_value()) {
            surface->destroy(instance.getVkInstance());
        }
        instance.destroy();
    }

    Instance SharedContext::createInstance(std::vector<std::tuple<const char *, bool>> instanceExtensions,
                                           std::vector<const char *> validationLayers,
                                           bool glfwSupport) {
        if (glfwSupport) {
            uint32_t glfwExtensionCount = 0;
            const char **glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            std::vector<const char *> surfaceExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

            std::transform(surfaceExtensions.begin(), surfaceExtensions.end(), std::back_inserter(instanceExtensions),
                           [](const char *name) {
                               return std::make_tuple(name, false);
                           }
            );
        }

        return Instance{instanceExtensions, std::move(validationLayers)};
    }

    Surface SharedContext::createSurface(Instance instance, GLFWwindow *window) {
        return Surface{instance.getVkInstance(), window};
    }

    Device SharedContext::createDevice(Instance instance, Surface surface,
                                       std::vector<std::tuple<const char *, bool>> deviceExtensions) {
        auto gpu = PhysicalDevice::enumeratePhysicalDevices(instance.getVkInstance()).front();
        deviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME, false);
        return Device{gpu, surface.getVkSurface(), deviceExtensions};
    }

    Instance SharedContext::getInstance() {
        return instance;
    }

    std::optional<Surface> SharedContext::getSurface() {
        return surface;
    }

    Device SharedContext::getDevice() {
        return device;
    }

}

