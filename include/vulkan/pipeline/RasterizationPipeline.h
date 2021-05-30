#ifndef VISION_RASTERIZATIONPIPELINE_H
#define VISION_RASTERIZATIONPIPELINE_H

#include <vulkan/vulkan.hpp>

namespace vulkan {
    class RasterizationPipeline;
}

class vulkan::RasterizationPipeline {
public:
    RasterizationPipeline();
    ~RasterizationPipeline();

    void recreate();


private:
    VkPipeline pipeline;

    VkPipelineLayout pipelineLayout;

};


#endif //VISION_RASTERIZATIONPIPELINE_H
