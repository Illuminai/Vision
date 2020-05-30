#ifndef VISION_SWAPCHAINSUPPORTINFO_H
#define VISION_SWAPCHAINSUPPORTINFO_H

#include <vulkan/vulkan.hpp>

class SwapchainSupportInfo {
public:
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

SwapchainSupportInfo querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);


#endif //VISION_SWAPCHAINSUPPORTINFO_H
