#include "vulkan/core/InFlightFrames.h"

namespace vulkan {


    InFlightFrames::InFlightFrames(std::shared_ptr<Context> context, int maxFramesInFlight) {
        imageAvailableSemaphores.resize(maxFramesInFlight);
        renderFinishedSemaphores.resize(maxFramesInFlight);
        fences.resize(maxFramesInFlight);

        for (int i = 0; i < maxFramesInFlight; ++i) {
            VkSemaphoreCreateInfo semaphoreInfo{};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            checkError(vkCreateSemaphore(context->getDevice().getVkDevice(), &semaphoreInfo, nullptr,
                                         &imageAvailableSemaphores[i]),
                       "InFlightFrames image available semaphore creation");

            checkError(vkCreateSemaphore(context->getDevice().getVkDevice(), &semaphoreInfo, nullptr,
                                         &renderFinishedSemaphores[i]),
                       "InFlightFrames render finished semaphore creation");

            VkFenceCreateInfo fenceInfo{};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

            checkError(vkCreateFence(context->getDevice().getVkDevice(), &fenceInfo, nullptr,
                                     &fences[i]),
                       "InFlightFrames fence creation");
        }
    }

    InFlightFrames::~InFlightFrames() {
        for (int i = 0; i < fences.size(); ++i) {
            vkDestroySemaphore(context->getDevice().getVkDevice(), renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(context->getDevice().getVkDevice(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(context->getDevice().getVkDevice(), fences[i], nullptr);
        }
    }

    SyncObject InFlightFrames::nextSyncObject() {
        VkSemaphore imageAvailableSemaphore = imageAvailableSemaphores[currentFrame];
        VkSemaphore renderFinishedSemaphore = renderFinishedSemaphores[currentFrame];
        VkFence fence = fences[currentFrame];

        currentFrame = (currentFrame + 1) % fences.size();

        return SyncObject{
                imageAvailableSemaphore,
                renderFinishedSemaphore,
                fence
        };
    }


}
