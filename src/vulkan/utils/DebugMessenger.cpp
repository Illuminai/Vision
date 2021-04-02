#include "vulkan/utils/DebugMessenger.h"

#include <iostream>
#include "vulkan/utils/VkInitializer.h"
#include "vulkan/utils/ErrorCheck.h"

namespace vulkan {

    DebugMessenger::DebugMessenger(VkInstance instance) {
        auto createInfo = getCreateInfo();
        checkError(createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger),
                   "Debug messenger creation");
    }

    void DebugMessenger::destroy(VkInstance instance) {
        if (debugMessenger) {
            destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
            debugMessenger = VK_NULL_HANDLE;
        }
    }

    VkBool32 DebugMessenger::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                           VkDebugUtilsMessageTypeFlagsEXT messageType,
                                           const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                           void *pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    VkResult DebugMessenger::createDebugUtilsMessengerEXT(VkInstance instance,
                                                          const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                          const VkAllocationCallbacks *pAllocator,
                                                          VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                               "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DebugMessenger::destroyDebugUtilsMessengerEXT(VkInstance instance,
                                                       VkDebugUtilsMessengerEXT debugMessenger,
                                                       const VkAllocationCallbacks *pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                                "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    VkDebugUtilsMessengerCreateInfoEXT DebugMessenger::getCreateInfo() {
        auto createInfo = VkInitializer::createDebugUtilsMessengerCreateInfoEXT();
        createInfo.messageSeverity = //VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT  /* Too many messages, pls stop */
                //|
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;

        return createInfo;
    }


}