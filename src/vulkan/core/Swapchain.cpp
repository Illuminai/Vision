#include "vulkan/core/Swapchain.h"

namespace vulkan {

    Swapchain::Swapchain(std::shared_ptr<Context> context, uint32_t preferredDimensions[2])
            : context(context) {
        if (!context->getSurface().has_value()) {
            throw std::runtime_error("Can't create swap chain without surface");
        }
        SwapchainSupportDetails details{context->getDevice().getVkPhysicalDevice(),
                                        context->getSurface()->getVkSurface()};

        surfaceCapabilities = details.getCapabilities();
        format = details.getOptimalSurfaceFormat();
        presentMode = details.getOptimalPresentMode();
        extent = details.getOptimalExtent(preferredDimensions);
        imageCount = details.getOptimalImageCount();

        createSwapchain();
    }

    Swapchain::~Swapchain() {
        destroy();
    }

    std::vector<VkFramebuffer> Swapchain::createFramebuffers(VkRenderPass renderPass) {
        std::vector<VkFramebuffer> framebuffers(imageViews.size());
        for (size_t i = 0; i < imageViews.size(); i++) {
            VkImageView attachments[] = {imageViews[i]};

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = extent.width;
            framebufferInfo.height = extent.height;
            framebufferInfo.layers = 1;

            checkError(vkCreateFramebuffer(context->getDevice().getVkDevice(), &framebufferInfo, nullptr,
                                           &framebuffers[i]),
                       "Swapchain framebuffer creation");
        }

        return framebuffers;
    }

    void Swapchain::rebuild(uint32_t preferredDimensions[2]) {
        destroy();
        SwapchainSupportDetails details{context->getDevice().getVkPhysicalDevice(),
                                        context->getSurface()->getVkSurface()};
        extent = details.getOptimalExtent(preferredDimensions);
        createSwapchain();
    }

    void Swapchain::destroy() {
        for (auto imageView : imageViews) {
            vkDestroyImageView(context->getDevice().getVkDevice(), imageView, nullptr);
        }
        imageViews.clear();
        if (swapchain) {
            vkDestroySwapchainKHR(context->getDevice().getVkDevice(), swapchain, nullptr);
        }
    }

    void Swapchain::createSwapchain() {
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = context->getSurface()->getVkSurface();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = format.format;
        createInfo.imageColorSpace = format.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        if (context->getDevice().getGraphicsQueueFamily() != context->getDevice().getPresentQueueFamily()) {
            uint32_t familyIndices[2] = {context->getDevice().getGraphicsQueueFamily(),
                                         context->getDevice().getPresentQueueFamily()};
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = familyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }
        createInfo.preTransform = surfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        checkError(vkCreateSwapchainKHR(context->getDevice().getVkDevice(), &createInfo, nullptr, &swapchain),
                   "Swapchain creation");

        vkGetSwapchainImagesKHR(context->getDevice().getVkDevice(), swapchain, &imageCount, nullptr);
        images.resize(imageCount);
        vkGetSwapchainImagesKHR(context->getDevice().getVkDevice(), swapchain, &imageCount, images.data());

        imageViews.resize(images.size());

        for (size_t i = 0; i < images.size(); i++) {
            VkImageViewCreateInfo imageViewCreateInfo{};
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.image = images[i];
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = format.format;
            imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;
            checkError(vkCreateImageView(context->getDevice().getVkDevice(), &imageViewCreateInfo, nullptr,
                                         &imageViews[i]),
                       "Swapchain image view creation");
        }
    }

}

namespace vulkan {

    SwapchainSupportDetails::SwapchainSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
        if (formatCount != 0) {
            formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
        if (presentModeCount != 0) {
            presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());
        }
    }

    VkSurfaceFormatKHR SwapchainSupportDetails::getOptimalSurfaceFormat() {
        for (auto format : formats) {
            if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return format;
            }
        }
        return formats[0];
    }

    VkPresentModeKHR SwapchainSupportDetails::getOptimalPresentMode() {
        for (const auto &presentMode : presentModes) {
            if (presentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR) {
                return presentMode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapchainSupportDetails::getOptimalExtent(uint32_t preferredDimensions[2]) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        }

        auto width = std::max(std::min(preferredDimensions[0], capabilities.maxImageExtent.width),
                              capabilities.minImageExtent.width);
        auto height = std::max(std::min(preferredDimensions[1], capabilities.maxImageExtent.height),
                               capabilities.minImageExtent.height);

        return VkExtent2D{width, height};
    }

    uint32_t SwapchainSupportDetails::getOptimalImageCount() {
        uint32_t maxImageCount = capabilities.maxImageCount;
        uint32_t preferredImageCount = capabilities.minImageCount + 1;

        if (maxImageCount > 0 && preferredImageCount > maxImageCount) {
            preferredImageCount = maxImageCount;
        }
        return preferredImageCount;
    }

    VkSurfaceCapabilitiesKHR SwapchainSupportDetails::getCapabilities() {
        return capabilities;
    }

    std::vector<VkSurfaceFormatKHR> SwapchainSupportDetails::getSurfaceFormats() {
        return formats;
    }

    std::vector<VkPresentModeKHR> SwapchainSupportDetails::getPresentModes() {
        return presentModes;
    }

}
