#include "vulkan/core/Instance.h"
#include "vulkan/core/PhysicalDevice.h"
#include <iostream>

int main() {

    const std::vector<const char *> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<std::tuple<const char *, bool>> instanceExtensions = {
            std::make_tuple(VK_KHR_SURFACE_EXTENSION_NAME, false),
            //std::make_tuple("OOGA", false), Extension not optional and not available
            std::make_tuple("OOF", true)
    };

    try {
        vulkan::Instance instance{instanceExtensions, validationLayers};

        auto gpus = vulkan::PhysicalDevice::enumeratePhysicalDevices(instance.getVkInstance());

        for (auto gpu : gpus) {
            VkPhysicalDeviceProperties prop;
            vkGetPhysicalDeviceProperties(gpu, &prop);
            std::cout << "GPU Name: " << prop.deviceName << std::endl;
        }

        auto gpu = gpus.front();

        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extensionCount, extensions.data());

        std::cout << "Extensions: " << std::endl;
        for(auto extension : extensions) {
            std::cout << "Extension: " << extension.extensionName << std::endl;
        }



    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }


    return 0;
}