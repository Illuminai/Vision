#ifndef VISION_PLATFORMWINDOW_H
#define VISION_PLATFORMWINDOW_H

#include "frontend/vulkan/VulkanApplication.h"

class PlatformWindow {
public:
    void initWindow();

    void loop();

    void destroyWindow();

private:
    VulkanApplication vkApplication;
    GLFWwindow *window;

};


#endif //VISION_PLATFORMWINDOW_H
