#ifndef VISION_INSTANCE_H
#define VISION_INSTANCE_H

#include <vulkan/vulkan.hpp>
#include <optional>
#include <vulkan/debug/DebugMessenger.h>

namespace vulkan {
    class Instance;
}

class vulkan::Instance {
public:
    Instance(std::vector<std::tuple<const char *, bool>> requiredExtensions, std::vector<const char *> validationLayers);

    void destroy();

    static bool isExtensionAvailable(const char *extensionName);

    static std::vector<VkExtensionProperties> getAvailableExtensions();

    static bool isLayerAvailable(const char *layerName);

    static std::vector<VkLayerProperties> getAvailableLayers();

    VkInstance getVkInstance();

    std::optional<DebugMessenger> getDebugMessenger();

private:
    VkInstance instance{VK_NULL_HANDLE};

    std::optional<DebugMessenger> debugMessenger;

};


#endif //VISION_INSTANCE_H
