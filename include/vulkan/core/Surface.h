#ifndef VISION_SURFACE_H
#define VISION_SURFACE_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <memory>
#include "vulkan/core/Instance.h"

namespace vulkan {
    class Surface;
}

class vulkan::Surface {
public:
    Surface(const std::shared_ptr<Instance> &instance, GLFWwindow *window);

    ~Surface();

    VkSurfaceKHR getVkSurface();

private:
    VkSurfaceKHR surface{VK_NULL_HANDLE};

    std::shared_ptr<Instance> instance;

};


#endif //VISION_SURFACE_H
