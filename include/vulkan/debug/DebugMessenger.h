#ifndef VISION_DEBUGMESSENGER_H
#define VISION_DEBUGMESSENGER_H

#include <vulkan/vulkan.hpp>

namespace vulkan {
    class DebugMessenger;
}

class vulkan::DebugMessenger {
public:
    DebugMessenger(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT *createInfo);

    void destroy(VkInstance instance);

    static VkDebugUtilsMessengerCreateInfoEXT getCreateInfo();

private:
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                        void *pUserData);

    static VkResult createDebugUtilsMessengerEXT(VkInstance instance,
                                                 const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                 const VkAllocationCallbacks *pAllocator,
                                                 VkDebugUtilsMessengerEXT *pDebugMessenger);

    static void destroyDebugUtilsMessengerEXT(VkInstance instance,
                                              VkDebugUtilsMessengerEXT debugMessenger,
                                              const VkAllocationCallbacks *pAllocator);

};


#endif //VISION_DEBUGMESSENGER_H
