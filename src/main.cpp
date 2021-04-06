#include "vulkan/core/Instance.h"
#include "vulkan/core/PhysicalDevice.h"
#include "vulkan/core/Context.h"
#include "vulkan/core/Swapchain.h"
#include <iostream>

int main() {

    try {

        //INIT GLFW
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW!");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        GLFWwindow *window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);

        if (!glfwVulkanSupported()) {
            throw std::runtime_error("Vulkan not supported!");
        }

        // INIT VULKAN
        std::vector<std::tuple<const char *, bool>> instanceExtensions = {
                //std::make_tuple("TESTTEST", false), Extension not optional and not available. will crash.
                std::make_tuple("OOF", true) // Extension optional
        };
        const std::vector<const char *> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        const std::vector<std::tuple<const char *, bool>> deviceExtensions = {
                std::make_tuple(VK_KHR_SWAPCHAIN_EXTENSION_NAME, false),
                //std::make_tuple("TESTTEST", false), Extension not optional and not available. will crash.
                std::make_tuple("OOF", true) // Extension optional
        };

        std::shared_ptr<vulkan::Context> context = std::make_shared<vulkan::Context>(instanceExtensions,
                                                                                     validationLayers, deviceExtensions,
                                                                                     window);

        uint32_t dim[2] = {1,1};
        vulkan::Swapchain swapchain{context, dim};


        // LOOP WINDOW
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            //DRAW ETC
        }


    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }


    return 0;
}