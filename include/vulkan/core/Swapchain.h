#ifndef VISION_SWAPCHAIN_H
#define VISION_SWAPCHAIN_H

#include <vulkan/vulkan.hpp>
#include "vulkan/core/Context.h"

namespace vulkan {
    class Swapchain;

    class SwapchainSupportDetails;
}

//TODO: Custom Swapchain
class vulkan::Swapchain {
public:
    Swapchain(std::shared_ptr<Context> context, uint32_t preferredDimensions[2]);

    ~Swapchain();

    std::vector<VkFramebuffer> createFramebuffers(VkRenderPass renderPass);


    void rebuild();

    void destroy();

    VkSwapchainKHR swapchain{VK_NULL_HANDLE};

    VkSurfaceCapabilitiesKHR surfaceCapabilities;

    VkSurfaceFormatKHR format;
    VkPresentModeKHR presentMode;
    VkExtent2D extent;
    uint32_t imageCount;

    std::vector<VkImageView> imageViews;

private:
    std::shared_ptr<Context> context;

    std::vector<VkImage> images;

    void createSwapchain();

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
