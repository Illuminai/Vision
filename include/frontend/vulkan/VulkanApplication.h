#ifndef VISION_VULKANAPPLICATION_H
#define VISION_VULKANAPPLICATION_H

#include <iostream>
#include <vector>
#include <set>
#include <fstream>

#include "VulkanInitializer.h"
#include "QueueFamilyIndices.h"
#include "SwapchainSupportInfo.h"
#include "core/math/Vector3.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const int WIDTH = 800, HEIGHT = 600;

const std::vector<const char *> validationLayers = {
        "VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class VulkanApplication {
public:
    explicit VulkanApplication(GLFWwindow *window) : window(window) {}

    VulkanApplication() = default;

    void initVulkan();

    void draw();

    void destroyVulkan();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags,
                                                        VkDebugReportObjectTypeEXT objType,
                                                        uint64_t obj,
                                                        size_t location,
                                                        int32_t code,
                                                        const char *layerPrefix,
                                                        const char *msg,
                                                        void *userData);

private:
    GLFWwindow *window;

    VkInstance instance;

    VkDebugReportCallbackEXT debugReportCallback;

    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice;

    VkDevice logicalDevice;

    VkQueue presentQueue;
    VkQueue computeQueue;

    VkSwapchainKHR swapchain;

    std::vector<VkImage> swapchainImages;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;

    std::vector<VkImageView> swapchainImageViews;

    VkImage computeImage;
    VkImageView computeImageView;
    VkDeviceMemory computeImageDeviceMemory;

    VkBuffer sphereBuffer;
    VkDeviceMemory sphereDeviceMemory;

    VkDescriptorPool computeDescriptorPool;
    VkDescriptorSetLayout computeDescriptorSetLayout;
    VkPipelineLayout computePipelineLayout;
    std::vector<VkDescriptorSet> computeDescriptorSets;

    VkPipeline computePipeline;
    VkCommandPool computeCommandPool;

    uint32_t currentImageIndex;
    VkCommandBuffer computeCommandBuffer;
    VkFence computeFence;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;

    // Creation functions

    void createInstance();

    void setupDebugCallback();

    void createSurface();

    void choosePhysicalDevice();

    void createLogicalDevice();

    void createSwapchain();

    void createImageViews();

    void createComputeImage(VkImage &image, VkImageView &imageView, VkDeviceMemory &memory);

    void prepareStorageBuffers();

    void createDescriptorPool();

    void prepareComputeForPipelineCreation();

    void createComputePipeline();

    void createComputeCommandPool();

    void createComputeCommandBuffer();

    void recordComputeCommandBuffer();

    void createComputeFence();

    void createSemaphores();

    // Helper functions

    bool validationLayersSupported();

    std::vector<const char *> getRequiredExtensions();

    void printSupportedExtensions();

    bool isDeviceSuitable(VkPhysicalDevice device);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR> availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    void getMemoryProperties(int &memoryTypeIndex, VkMemoryPropertyFlags properties);

    void createStorageBuffer(void *data, VkDeviceSize &bufferSize, VkBuffer &buffer,
                             VkBufferUsageFlags bufferUsageFlags, VkDeviceMemory &deviceMemory,
                             uint32_t memoryTypeIndex);

    void copyMemory(const void *data, VkDeviceMemory &deviceMemory, VkDeviceSize &bufferSize);

    std::vector<char> readBinaryFile(const std::string &filename);

    void createShaderModule(const std::vector<char> &code, VkShaderModule &shaderModule);

    void setFirstImageBarriers(VkCommandBuffer buffer);

    void copyImageMemory(VkCommandBuffer buffer);

    void setSecondImageBarriers(VkCommandBuffer buffer);

    // Own creation function

    VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugReportCallbackEXT *pCallback);


};

#endif //VISION_VULKANAPPLICATION_H
