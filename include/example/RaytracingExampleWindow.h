#ifndef VISION_RAYTRACINGEXAMPLEWINDOW_H
#define VISION_RAYTRACINGEXAMPLEWINDOW_H

#include "windowing/VulkanWindow.h"


class RaytracingExampleWindow : public windowing::VulkanWindow {
public:
    RaytracingExampleWindow();

    ~RaytracingExampleWindow();

    void onRender(vulkan::SyncObject syncObject, uint32_t imageIndex) override;

    void onSwapchainRebuild() override;

private:
    VkRenderPass renderPass{VK_NULL_HANDLE};

    std::vector<VkFramebuffer> framebuffers;

    void createRenderPass();
};


#endif //VISION_RAYTRACINGEXAMPLEWINDOW_H
