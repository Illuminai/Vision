#ifndef VISION_SHADERMODULE_H
#define VISION_SHADERMODULE_H

#include <vulkan/vulkan.hpp>
#include "vulkan/debug/ErrorCheck.h"
#include "vulkan/core/Context.h"

namespace vulkan {
    class ShaderModule;
}

class vulkan::ShaderModule {
public:
    ShaderModule(std::shared_ptr<Context> context, std::vector<uint32_t> shaderCode);

    void destroy();

    VkShaderModule getVkShaderModule();

private:
    std::shared_ptr<Context> context;

    VkShaderModule shaderModule{VK_NULL_HANDLE};

};


#endif //VISION_SHADERMODULE_H
