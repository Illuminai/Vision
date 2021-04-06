#ifndef VISION_CONTEXT_H
#define VISION_CONTEXT_H

#include <memory>
#include "vulkan/core/SharedContext.h"
#include "vulkan/core/CommandPool.h"

#include "vulkan/debug/ErrorCheck.h"
#include <type_traits>
#include <utility>
#include <iostream>

namespace vulkan {
    class Context;
}

class vulkan::Context {
public:
    Context(const std::vector<std::tuple<const char *, bool>> &instanceExtensions,
            const std::vector<const char *> &validationLayers,
            const std::vector<std::tuple<const char *, bool>> &deviceExtensions,
            GLFWwindow *window);

    explicit Context(std::shared_ptr<SharedContext> sharedContext);

    ~Context();

    template<typename F>
    auto executeTransient(F const &commands) {
        VkCommandBuffer commandBuffer = beginTransientExecution();
        auto result = commands(commandBuffer);
        endTransientExecution(commandBuffer);
        return result;
    }

    std::shared_ptr<SharedContext> getSharedContext();

    Instance getInstance();

    std::optional<Surface> getSurface();

    Device getDevice();

private:
    std::shared_ptr<SharedContext> sharedContext;

    std::optional<CommandPool> generalCommandPool;
    std::optional<CommandPool> transientCommandPool;

    VkCommandBuffer beginTransientExecution();

    void endTransientExecution(VkCommandBuffer commandBuffer);

};


#endif //VISION_CONTEXT_H
