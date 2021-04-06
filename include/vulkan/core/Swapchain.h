#ifndef VISION_SWAPCHAIN_H
#define VISION_SWAPCHAIN_H

#include <vulkan/vulkan.hpp>
#include "vulkan/core/Context.h"

namespace vulkan {
    class Swapchain;

    class SwapchainSupportDetails;
}


class vulkan::Swapchain {
public:
    Swapchain(std::shared_ptr<Context> context, uint32_t preferredDimensions[2]);

    ~Swapchain();

private:
    std::shared_ptr<Context> context;

    VkSwapchainKHR swapchain{VK_NULL_HANDLE};

    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    std::vector<VkFramebuffer> framebuffer;

    VkSurfaceFormatKHR format;
    VkPresentModeKHR presentMode;
    VkExtent2D extent;
    uint32_t imageCount;

};


class vulkan::SwapchainSupportDetails {
public:
    SwapchainSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    VkSurfaceFormatKHR getOptimalSurfaceFormat();

    VkPresentModeKHR getOptimalPresentMode();

    VkExtent2D getOptimalExtent(uint32_t preferredDimensions[2]);

    uint32_t getOptimalImageCount();

    VkSurfaceCapabilitiesKHR getCapabilities();

    std::vector<VkSurfaceFormatKHR> getSurfaceFormats();

    std::vector<VkPresentModeKHR> getPresentModes();


private:
    VkSurfaceCapabilitiesKHR capabilities{};

    std::vector<VkSurfaceFormatKHR> formats;

    std::vector<VkPresentModeKHR> presentModes;

};


#endif //VISION_SWAPCHAIN_H
