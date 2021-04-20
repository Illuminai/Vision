#include "vulkan/core/Instance.h"

#include "vulkan/debug/ErrorCheck.h"

namespace vulkan {

    Instance::Instance(std::vector<std::tuple<const char *, bool>> requiredExtensions,
                       std::vector<const char *> validationLayers) {

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vision";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Vision Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        bool enableDebug = false;
        if (!validationLayers.empty() && isExtensionAvailable(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)) {
            auto iterator = std::find_if(requiredExtensions.begin(), requiredExtensions.end(),
                                         [](std::tuple<const char *, bool> tuple) {
                                             return strcmp(std::get<0>(tuple), VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0;
                                         }
            );
            if (iterator == requiredExtensions.end()) {
                requiredExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, true);
            }
            enableDebug = true;
        }

        std::vector<const char *> layers;
        if (enableDebug) {
            std::copy_if(validationLayers.begin(), validationLayers.end(), std::back_inserter(layers),
                         [](const char *layer) { return isLayerAvailable(layer); }
            );
        }
        createInfo.enabledLayerCount = layers.size();
        createInfo.ppEnabledLayerNames = layers.data();

        std::vector<const char *> extensions;
        for (auto tuple : requiredExtensions) {
            bool available = isExtensionAvailable(std::get<0>(tuple));
            if (!std::get<1>(tuple) && !available) {
                std::string buffer("Instance extension ");
                buffer.append(std::get<0>(tuple));
                buffer.append(" not present");
                throw std::runtime_error(buffer);
            }
            if (available) {
                extensions.push_back(std::get<0>(tuple));
            }
        }

        createInfo.enabledExtensionCount = extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();

        if (!layers.empty()) {
            auto debugCreateInfo = DebugMessenger::getCreateInfo();
            createInfo.pNext = &debugCreateInfo;
            checkError(vkCreateInstance(&createInfo, nullptr, &instance), "Instance creation");
            debugMessenger = DebugMessenger{instance, &debugCreateInfo};
        }else{
            checkError(vkCreateInstance(&createInfo, nullptr, &instance), "Instance creation");
        }

    }

    void Instance::destroy() {
        if (debugMessenger.has_value()) {
            debugMessenger->destroy(instance);
            debugMessenger.reset();
        }
        if (instance) {
            vkDestroyInstance(instance, nullptr);
            instance = VK_NULL_HANDLE;
        }
    }

    bool Instance::isExtensionAvailable(const char *extensionName) {
        auto presentExtensions = getAvailableExtensions();
        bool found = false;
        for (auto extension : presentExtensions) {
            if (strcmp(extension.extensionName, extensionName) == 0) {
                found = true;
            }
        }
        return found;
    }

    std::vector<VkExtensionProperties> Instance::getAvailableExtensions() {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        return extensions;
    }

    bool Instance::isLayerAvailable(const char *layerName) {
        auto presentExtensions = getAvailableLayers();
        bool found = false;
        for (auto extension : presentExtensions) {
            if (strcmp(extension.layerName, layerName) == 0) {
                found = true;
            }
        }
        return found;
    }

    std::vector<VkLayerProperties> Instance::getAvailableLayers() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        return availableLayers;
    }

    VkInstance Instance::getVkInstance() {
        return instance;
    }

    std::optional<DebugMessenger> Instance::getDebugMessenger() {
        return debugMessenger;
    }

}