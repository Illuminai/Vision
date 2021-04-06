#ifndef VISION_SHAREDCONTEXT_H
#define VISION_SHAREDCONTEXT_H

#include "vulkan/core/Instance.h"
#include "vulkan/core/Device.h"

namespace vulkan {
    class SharedContext;
}

class vulkan::SharedContext {
public:
    SharedContext(const std::vector<std::tuple<const char *, bool>>& instanceExtensions,
                  const std::vector<const char *>& validationLayers,
                  std::vector<std::tuple<const char *, bool>> deviceExtensions,
                  GLFWwindow *window);

    ~SharedContext();

    Instance getInstance();

    std::optional<Surface> getSurface();

    Device getDevice();

private:
    Instance instance;
    std::optional<Surface> surface;
    Device device;

    static Instance createInstance(std::vector<std::tuple<const char *, bool>> instanceExtensions,
                            std::vector<const char *> validationLayers,
                            bool glfwSupport);

    static Surface createSurface(Instance instance, GLFWwindow *window);

    static Device createDevice(Instance instance,Surface surface, std::vector<std::tuple<const char *, bool>> deviceExtensions);


};


#endif //VISION_SHAREDCONTEXT_H
