#ifndef VISION_INFLIGHTFRAMES_H
#define VISION_INFLIGHTFRAMES_H

#include "vulkan/core/Context.h"

namespace vulkan {
    class InFlightFrames;

    struct SyncObject;
}

class vulkan::InFlightFrames {
public:
    InFlightFrames(std::shared_ptr<Context> context, int maxFramesInFlight);

    ~InFlightFrames();

    SyncObject nextSyncObject();

private:
    std::shared_ptr<Context> context;

    int currentFrame;

    std::vector<VkSemaphore> imageAvailableSemaphores;

    std::vector<VkSemaphore> renderFinishedSemaphores;

    std::vector<VkFence> fences;

};


struct vulkan::SyncObject {
    VkSemaphore imageAvailableSemaphore;

    VkSemaphore renderFinishedSemaphore;

    VkFence fence;
};

#endif //VISION_INFLIGHTFRAMES_H
