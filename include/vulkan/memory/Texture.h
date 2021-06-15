#ifndef VISION_TEXTURE_H
#define VISION_TEXTURE_H

#include "vulkan/core/Context.h"

namespace vulkan {
    class Texture;
}

class vulkan::Texture {
public:


private:
    std::shared_ptr<Context> context;

    //TODO: Image Object own

    VkImageView imageView{VK_NULL_HANDLE};

    std::optional<VkSampler> sampler;
};


#endif //VISION_TEXTURE_H
