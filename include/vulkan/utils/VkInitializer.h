#ifndef VISION_VKINITIALIZER_H
#define VISION_VKINITIALIZER_H

namespace vulkan::VkInitializer {

    inline VkApplicationInfo createApplicationInfo(const char *appName, uint32_t appVersion, uint32_t apiVersion) {
        VkApplicationInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        vk.pApplicationName = appName;
        vk.applicationVersion = appVersion;
        vk.pEngineName = "Vision Engine";
        vk.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        vk.apiVersion = apiVersion;
        return vk;
    }

    inline VkInstanceCreateInfo createInstanceCreateInfo(const VkApplicationInfo appInfo) {
        VkInstanceCreateInfo vk{};
        vk.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vk.pApplicationInfo = &appInfo;
        return vk;
    }

    inline VkDebugUtilsMessengerCreateInfoEXT createDebugUtilsMessengerCreateInfoEXT() {
        VkDebugUtilsMessengerCreateInfoEXT vk{};
        vk.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        return vk;
    }


}


#endif //VISION_VKINITIALIZER_H
