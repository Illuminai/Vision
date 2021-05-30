#include "vulkan/shader/ShaderModule.h"

namespace vulkan {

    ShaderModule::ShaderModule(std::shared_ptr<Context> context, std::vector<uint32_t> shaderCode) : context(context) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shaderCode.size() * sizeof(uint32_t);
        createInfo.pCode = shaderCode.data();

        checkError(vkCreateShaderModule(context->getDevice().getVkDevice(), &createInfo, nullptr, &shaderModule),
                   "Shader module creation");
    }

    void ShaderModule::destroy() {
        if (shaderModule) {
            vkDestroyShaderModule(context->getDevice().getVkDevice(), shaderModule, nullptr);
        }
    }

    VkShaderModule ShaderModule::getVkShaderModule() {
        return shaderModule;
    }

}

