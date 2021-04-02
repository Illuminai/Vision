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
    Surface(VkInstance instance, GLFWwindow *window);

    void destroy(VkInstance instance);

    VkSurfaceKHR getVkSurface();

private:
    VkSurfaceKHR surface{VK_NULL_HANDLE};

};


#endif //VISION_SURFACE_H
