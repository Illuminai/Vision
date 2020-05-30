#include "frontend/vulkan/VulkanApplication.h"
#include <vrc.h>

void VulkanApplication::initVulkan() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    std::cout << width << height << std::endl;

    createInstance();
    setupDebugCallback();
    createSurface();
    choosePhysicalDevice();
    createLogicalDevice();

    createSwapchain();
    createImageViews();

    createComputeImage(computeImage, computeImageView, computeImageDeviceMemory);
    prepareStorageBuffers();

    createDescriptorPool();
    prepareComputeForPipelineCreation();
    createComputePipeline();

    createComputeCommandPool();
    createComputeCommandBuffer();
    recordComputeCommandBuffer();
    createComputeFence();

    createSemaphores();
}


void VulkanApplication::draw() {
    vkAcquireNextImageKHR(logicalDevice, swapchain, (std::numeric_limits<uint64_t>::max)(), imageAvailableSemaphore,
                          VK_NULL_HANDLE, &currentImageIndex);

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    VkSubmitInfo computeSubmitInfo = {};
    computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    computeSubmitInfo.commandBufferCount = 1;
    computeSubmitInfo.pCommandBuffers = &computeCommandBuffer;

    computeSubmitInfo.waitSemaphoreCount = 1;
    computeSubmitInfo.pWaitSemaphores = waitSemaphores;
    computeSubmitInfo.signalSemaphoreCount = 1;
    computeSubmitInfo.pSignalSemaphores = signalSemaphores;
    computeSubmitInfo.pWaitDstStageMask = waitStages;

    vkWaitForFences(logicalDevice, 1, &computeFence, VK_TRUE, UINT64_MAX);
    vkResetFences(logicalDevice, 1, &computeFence);

    recordComputeCommandBuffer();

    if (vkQueueSubmit(computeQueue, 1, &computeSubmitInfo, computeFence) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit compute command buffers to compute queue!");
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &currentImageIndex;

    if (vkQueuePresentKHR(presentQueue, &presentInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swapchain image!");
    }
}


void VulkanApplication::destroyVulkan() {
    vkDestroyInstance(instance, nullptr);
}


void VulkanApplication::createInstance() {
    if (enableValidationLayers && !validationLayersSupported()) {
        throw std::runtime_error("Validation layers not supported!");
    }

    auto appInfo = VulkanInitializer::createApplicationInfo("MandelbrotVK",
                                                            VK_MAKE_VERSION(1, 0, 0),
                                                            VK_API_VERSION_1_0);

    auto instanceInfo = VulkanInitializer::createInstanceCreateInfo(appInfo);

    auto extensions = getRequiredExtensions();
    instanceInfo.enabledExtensionCount = extensions.size();
    instanceInfo.ppEnabledExtensionNames = extensions.data();

    if (enableValidationLayers) {
        instanceInfo.enabledLayerCount = validationLayers.size();
        instanceInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        instanceInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }
}


void VulkanApplication::setupDebugCallback() {
    if (!enableValidationLayers) {
        return;
    }

    VkDebugReportCallbackCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = VulkanApplication::debugCallback;

    if (createDebugReportCallbackEXT(instance, &createInfo, nullptr, &debugReportCallback) != VK_SUCCESS) {
        throw std::runtime_error("Failed to setup debug callback!");
    }
}


void VulkanApplication::createSurface() {
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}


void VulkanApplication::choosePhysicalDevice() {
    uint32_t deviceCount;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPU with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    physicalDevice = VK_NULL_HANDLE;
    for (const auto &device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find suitable GPU!");
    }
}


void VulkanApplication::createLogicalDevice() {
    auto indices = findQueueFamilies(physicalDevice, surface);
    std::vector<VkDeviceQueueCreateInfo> queueInfos;
    std::set<int> uniqueQueueFamilies = {indices.presentFamily, indices.computeFamily};

    float queuePriority = 1.0f;
    for (int queueFamily : uniqueQueueFamilies) {
        auto queueCreateInfo = VulkanInitializer::createDeviceQueueCreateInfo(queueFamily, queuePriority);
        queueInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

    auto deviceInfo = VulkanInitializer::createDeviceCreateInfo();
    deviceInfo.pQueueCreateInfos = queueInfos.data();
    deviceInfo.queueCreateInfoCount = queueInfos.size();
    deviceInfo.pEnabledFeatures = &deviceFeatures;
    deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceInfo.enabledExtensionCount = deviceExtensions.size();

    if (enableValidationLayers) {
        deviceInfo.enabledLayerCount = validationLayers.size();
        deviceInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        deviceInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        VkPhysicalDeviceProperties deviceProperties{};
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
        std::string message = "Failed to create a logical device for the physical device: " +
                              std::string(deviceProperties.deviceName);
        throw std::runtime_error(message);
    }

    vkGetDeviceQueue(logicalDevice, indices.presentFamily, 0, &presentQueue);
    vkGetDeviceQueue(logicalDevice, indices.computeFamily, 0, &computeQueue);
}


void VulkanApplication::createSwapchain() {
    auto swapchainSupport = querySwapchainSupport(physicalDevice, surface);
    auto surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.formats);
    auto presentMode = chooseSwapPresentMode(swapchainSupport.presentModes);
    auto extent = chooseSwapExtent(swapchainSupport.capabilities);

    if (!(swapchainSupport.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT)) {
        throw std::runtime_error("Swapchain doesn't support VK_IMAGE_USAGE_STORAGE_BIT!");
    }

    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;

    if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount) {
        imageCount = swapchainSupport.capabilities.maxImageCount;
    }

    auto swapchainInfo = VulkanInitializer::createSwapchainCreateInfoKHR(surface);
    swapchainInfo.minImageCount = imageCount;
    swapchainInfo.imageFormat = surfaceFormat.format;
    swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapchainInfo.imageExtent = extent;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    swapchainInfo.preTransform = swapchainSupport.capabilities.currentTransform;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.presentMode = presentMode;
    swapchainInfo.clipped = VK_TRUE;

    swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

    VkSwapchainKHR newSwapchain;
    if (vkCreateSwapchainKHR(logicalDevice, &swapchainInfo, nullptr, &newSwapchain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swapchain!");
    }

    swapchain = newSwapchain;

    vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, swapchainImages.data());

    swapchainImageFormat = surfaceFormat.format;
    swapchainExtent = extent;
}


void VulkanApplication::createImageViews() {
    swapchainImageViews.resize(swapchainImages.size());
    for (int i = 0; i < swapchainImages.size(); i++) {
        VkComponentMapping components = {
                VK_COMPONENT_SWIZZLE_R,
                VK_COMPONENT_SWIZZLE_G,
                VK_COMPONENT_SWIZZLE_B,
                VK_COMPONENT_SWIZZLE_A
        };

        auto createInfo = VulkanInitializer::createImageViewCreateInfo(swapchainImages[i], VK_IMAGE_VIEW_TYPE_2D);
        createInfo.format = swapchainImageFormat;
        createInfo.components = components;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(logicalDevice, &createInfo, nullptr, &swapchainImageViews[i])) {
            throw std::runtime_error("Failed to create image view!");
        }
    }
}


void VulkanApplication::createComputeImage(VkImage &image, VkImageView &imageView, VkDeviceMemory &memory) {
    auto swapchainSupport = querySwapchainSupport(physicalDevice, surface);
    auto surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.formats);

    auto info = VulkanInitializer::createImageCreateInfo(VK_IMAGE_TYPE_2D);
    info.format = surfaceFormat.format;
    info.extent = {WIDTH, HEIGHT, 1};
    info.mipLevels = 1;
    info.arrayLayers = 1;
    info.samples = VK_SAMPLE_COUNT_1_BIT;
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    info.usage = VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    if (vkCreateImage(logicalDevice, &info, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create compute image!");
    }

    int memoryTypeIndex = 0;
    getMemoryProperties(memoryTypeIndex, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(logicalDevice, image, &memoryRequirements);

    auto memoryInfo = VulkanInitializer::createMemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex);
    if (vkAllocateMemory(logicalDevice, &memoryInfo, nullptr, &memory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate memory for compute image!");
    }
    if (vkBindImageMemory(logicalDevice, image, memory, 0) != VK_SUCCESS) {
        throw std::runtime_error("Failed to bind memory to compute image!");
    }

    auto viewInfo = VulkanInitializer::createImageViewCreateInfo(image, VK_IMAGE_VIEW_TYPE_2D);
    viewInfo.format = surfaceFormat.format;
    viewInfo.components = {VK_COMPONENT_SWIZZLE_R,
                           VK_COMPONENT_SWIZZLE_G,
                           VK_COMPONENT_SWIZZLE_B,
                           VK_COMPONENT_SWIZZLE_A};
    viewInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT,
                                 0, 1, 0, 1};

    if (vkCreateImageView(logicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create compute image view");
    }
}


void VulkanApplication::prepareStorageBuffers() {
    std::vector<Vector3d> spheres;
    //init objs
    spheres.emplace_back(1, 1, 1);

    int memoryTypeIndex = 0;
    getMemoryProperties(memoryTypeIndex, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

    VkDeviceSize sphereBufferSize = spheres.size() * sizeof(Vector3d);
    createStorageBuffer(spheres.data(), sphereBufferSize, sphereBuffer, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                        sphereDeviceMemory, memoryTypeIndex);
}

void VulkanApplication::createDescriptorPool() {
    auto storageSize = VulkanInitializer::createDescriptorPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 3);
    auto bufferSize = VulkanInitializer::createDescriptorPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2);
    auto uniformSize = VulkanInitializer::createDescriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1);

    std::vector<VkDescriptorPoolSize> poolSizes = {storageSize, bufferSize, uniformSize};

    auto poolInfo = VulkanInitializer::createDescriptorPoolCreateInfo();
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();

    if (vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &computeDescriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create compute descriptor pool!");
    }

}


void VulkanApplication::prepareComputeForPipelineCreation() {
    auto computeBinding = VulkanInitializer::createDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                                                                              VK_SHADER_STAGE_COMPUTE_BIT, 0);
    auto sphereBinding = VulkanInitializer::createDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                                                                             VK_SHADER_STAGE_COMPUTE_BIT, 1);

    std::vector<VkDescriptorSetLayoutBinding> bindings{computeBinding, sphereBinding};

    auto layoutInfo = VulkanInitializer::createDescriptorSetLayoutCreateInfo();
    layoutInfo.bindingCount = bindings.size();
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &computeDescriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create compute descriptor set layout!");
    }

    auto pipelineLayoutInfo = VulkanInitializer::createPipelineLayoutCreateInfo();
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &computeDescriptorSetLayout;

    if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &computePipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create compute pipeline layout!");
    }

    auto allocInfo = VulkanInitializer::createDescriptorSetAllocateInfo(computeDescriptorPool);
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &computeDescriptorSetLayout;

    computeDescriptorSets.resize(1);
    if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, computeDescriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate compute descriptor sets from compute descriptor pool!");
    }

    auto computeInfo = VulkanInitializer::createDescriptorImageInfo(computeImageView, VK_IMAGE_LAYOUT_GENERAL);
    auto sphereInfo = VulkanInitializer::createDescriptorBufferInfo(sphereBuffer);

    auto computeWrite = VulkanInitializer::createWriteDescriptorSet(computeDescriptorSets[0], 0,
                                                                    VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, &computeInfo);
    auto sphereWrite = VulkanInitializer::createWriteDescriptorSet(computeDescriptorSets[0], 1,
                                                                   VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &sphereInfo);

    std::vector<VkWriteDescriptorSet> writeSets = {computeWrite, sphereWrite};
    vkUpdateDescriptorSets(logicalDevice, writeSets.size(), writeSets.data(), 0, VK_NULL_HANDLE);
}


void VulkanApplication::createComputePipeline() {
    //auto computeShaderCode = readBinaryFile("shaders/comp.spv");
    auto computeShaderData = VRC_LOAD_RESOURCE(rt_comp_spv);
    auto computeShaderCode = std::vector<char>(computeShaderData.data(),
                                               computeShaderData.data() + computeShaderData.size());
    VkShaderModule computeShaderModule;
    createShaderModule(computeShaderCode, computeShaderModule);

    auto computeStageInfo = VulkanInitializer::createPipelineShaderStageCreateInfo();
    computeStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    computeStageInfo.module = computeShaderModule;
    computeStageInfo.pName = "main";

    auto pipelineInfo = VulkanInitializer::createComputePipelineCreateInfo();
    pipelineInfo.stage = computeStageInfo;
    pipelineInfo.layout = computePipelineLayout;

    if (vkCreateComputePipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &computePipeline) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to create compute pipelines!");
    }
}


void VulkanApplication::createComputeCommandPool() {
    auto queueFamilyIndices = findQueueFamilies(physicalDevice, surface);

    auto poolInfo = VulkanInitializer::createCommandPoolCreateInfo(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    poolInfo.queueFamilyIndex = queueFamilyIndices.computeFamily;

    if (vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &computeCommandPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create compute command pool!");
    }
}


void VulkanApplication::createComputeCommandBuffer() {
    currentImageIndex = 0;
    auto allocateInfo = VulkanInitializer::createCommandBufferAllocateInfo(computeCommandPool,
                                                                           VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    if (vkAllocateCommandBuffers(logicalDevice, &allocateInfo, &computeCommandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate compute command buffers!");
    }
}


void VulkanApplication::recordComputeCommandBuffer() {
    auto beginInfo = VulkanInitializer::createCommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
    if (vkBeginCommandBuffer(computeCommandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Compute command buffer recording couldn't be started!");
    }

    vkCmdBindPipeline(computeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);

    vkCmdBindDescriptorSets(computeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipelineLayout,
                            0, 1, &computeDescriptorSets[0], 0, 0);

    vkCmdDispatch(computeCommandBuffer, swapchainExtent.width, swapchainExtent.height, 1);

    setFirstImageBarriers(computeCommandBuffer);

    copyImageMemory(computeCommandBuffer);

    setSecondImageBarriers(computeCommandBuffer);

    if (vkEndCommandBuffer(computeCommandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Compute command buffer recording couldn't be ended!");
    }
}


void VulkanApplication::createComputeFence() {
    auto info = VulkanInitializer::createFenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
    vkCreateFence(logicalDevice, &info, nullptr, &computeFence);
}


void VulkanApplication::createSemaphores() {
    auto semaphoreInfo = VulkanInitializer::createSemaphoreCreateInfo();
    if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create semaphores!");
    }
}


bool VulkanApplication::validationLayersSupported() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : validationLayers) {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            return false;
        }
    }
    return true;
}

std::vector<const char *> VulkanApplication::getRequiredExtensions() {
    std::vector<const char *> extensions;

    unsigned int glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (int i = 0; i < glfwExtensionCount; i++) {
        extensions.push_back(glfwExtensions[i]);
    }

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return extensions;
}

void VulkanApplication::printSupportedExtensions() {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "Available extensions:" << std::endl;

    for (const auto &extension : extensions) {
        std::cout << "\t" << extension.extensionName << std::endl;
    }
}


bool VulkanApplication::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(device, surface);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapchainAdequate = false;
    if (extensionsSupported) {
        SwapchainSupportInfo swapchainSupport = querySwapchainSupport(device, surface);
        swapchainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
    }
    return indices.isComplete() && extensionsSupported && swapchainAdequate;
}

bool VulkanApplication::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    for (const auto &extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}


VkSurfaceFormatKHR
VulkanApplication::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }
    for (const auto &availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR VulkanApplication::chooseSwapPresentMode(std::vector<VkPresentModeKHR> availablePresentModes) {
    for (const auto &availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}


VkExtent2D VulkanApplication::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)()) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actualExtent = {WIDTH, HEIGHT};
        actualExtent.width = (std::max)(capabilities.minImageExtent.width,
                                        (std::min)(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = (std::max)(capabilities.minImageExtent.height,
                                         (std::min)(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}


void VulkanApplication::getMemoryProperties(int &memoryTypeIndex, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

    for (int i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            memoryTypeIndex = i;
            break;
        }
    }
}

void VulkanApplication::createStorageBuffer(void *data, VkDeviceSize &bufferSize,
                                            VkBuffer &buffer, VkBufferUsageFlags bufferUsageFlags,
                                            VkDeviceMemory &deviceMemory, uint32_t memoryTypeIndex) {
    auto bufferInfo = VulkanInitializer::createBufferCreateInfo(bufferUsageFlags);
    bufferInfo.size = bufferSize;

    if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create storage buffer!");
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(logicalDevice, buffer, &memoryRequirements);

    auto memoryInfo = VulkanInitializer::createMemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex);

    if (vkAllocateMemory(logicalDevice, &memoryInfo, nullptr, &deviceMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate device memory for storage buffer");
    }

    copyMemory(data, deviceMemory, bufferSize);
    vkBindBufferMemory(logicalDevice, buffer, deviceMemory, 0);
}


void VulkanApplication::copyMemory(const void *data, VkDeviceMemory &deviceMemory, VkDeviceSize &bufferSize) {
    void *mapped = nullptr;
    if (vkMapMemory(logicalDevice, deviceMemory, 0, VK_WHOLE_SIZE, 0, &mapped) != VK_SUCCESS) {
        throw std::runtime_error("Failed to map memory for storage buffer");
    }

    std::memcpy(mapped, data, bufferSize);

    if (mapped) {
        vkUnmapMemory(logicalDevice, deviceMemory);
        mapped = nullptr;
    }
}


std::vector<char> VulkanApplication::readBinaryFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open binary file !");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}


void VulkanApplication::createShaderModule(const std::vector<char> &code, VkShaderModule &shaderModule) {
    auto createInfo = VulkanInitializer::createShaderModuleCreateInfo();
    createInfo.codeSize = code.size();
    createInfo.pCode = (uint32_t *) code.data();

    if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module!");
    }
}


void VulkanApplication::setFirstImageBarriers(VkCommandBuffer buffer) {
    auto compWrite = VulkanInitializer::createImageMemoryBarrier(computeImage, VK_IMAGE_LAYOUT_UNDEFINED,
                                                                 VK_IMAGE_LAYOUT_GENERAL);
    compWrite.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    compWrite.srcAccessMask = 0;
    compWrite.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;

    auto compTransfer = VulkanInitializer::createImageMemoryBarrier(computeImage, VK_IMAGE_LAYOUT_GENERAL,
                                                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    compTransfer.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    compTransfer.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
    compTransfer.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

    auto swapTransfer = VulkanInitializer::createImageMemoryBarrier(swapchainImages[currentImageIndex],
                                                                    VK_IMAGE_LAYOUT_UNDEFINED,
                                                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    swapTransfer.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT,
                                     0, 1, 0, 1};
    swapTransfer.srcAccessMask = 0;
    swapTransfer.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    std::vector<VkImageMemoryBarrier> barriers{compWrite, compTransfer, swapTransfer};

    vkCmdPipelineBarrier(buffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                         0, 0, nullptr, 0, nullptr, barriers.size(), barriers.data());
}


void VulkanApplication::copyImageMemory(VkCommandBuffer buffer) {
    VkImageSubresourceLayers source;
    source.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    source.mipLevel = 0;
    source.baseArrayLayer = 0;
    source.layerCount = 1;

    VkImageSubresourceLayers dest;
    dest.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    dest.mipLevel = 0;
    dest.baseArrayLayer = 0;
    dest.layerCount = 1;

    VkImageCopy copy;
    copy.srcSubresource = source;
    copy.dstSubresource = dest;
    copy.extent = {WIDTH, HEIGHT, 1};
    copy.srcOffset = {0, 0, 0};
    copy.dstOffset = {0, 0, 0};


    vkCmdCopyImage(buffer, computeImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                   swapchainImages[currentImageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                   1, &copy);
}


void VulkanApplication::setSecondImageBarriers(VkCommandBuffer buffer) {
    auto swapPres = VulkanInitializer::createImageMemoryBarrier(swapchainImages[currentImageIndex],
                                                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    swapPres.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    swapPres.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    swapPres.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;

    vkCmdPipelineBarrier(buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                         0, 0, nullptr, 0,
                         nullptr, 1, &swapPres);
}


VkBool32 VKAPI_CALL
VulkanApplication::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj,
                                 size_t location, int32_t code, const char *layerPrefix, const char *msg,
                                 void *userData) {
    std::cerr << "Validation layer: " << msg << std::endl;

    return VK_FALSE;
}


VkResult
VulkanApplication::createDebugReportCallbackEXT(VkInstance instance,
                                                const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                                const VkAllocationCallbacks *pAllocator,
                                                VkDebugReportCallbackEXT *pCallback) {
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance,
                                                                           "vkCreateDebugReportCallbackEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}