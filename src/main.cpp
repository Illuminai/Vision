#include "vulkan/core/Instance.h"
#include "vulkan/core/PhysicalDevice.h"
#include "vulkan/core/Context.h"
#include "vulkan/core/Swapchain.h"
#include "vulkan/debug/ErrorCheck.h"
#include "windowing/ImGuiWindow.h"
#include <iostream>
#include <cmrc/cmrc.hpp>


#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <implot.h>

CMRC_DECLARE(tempshader);


int main() {

    windowing::ImGuiWindow window{};
    window.loopWindow();

    return 0;
}