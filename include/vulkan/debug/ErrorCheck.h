#ifndef VISION_ERRORCHECK_H
#define VISION_ERRORCHECK_H

#include <vulkan/vulkan.hpp>

namespace vulkan {
    const char *getErrorCode(VkResult result);

    void checkError(VkResult result, const char *operation);
}
#endif //VISION_ERRORCHECK_H
