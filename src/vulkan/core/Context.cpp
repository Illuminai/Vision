#include "vulkan/core/Context.h"

#include <utility>

namespace vulkan {

    std::shared_ptr<SharedContext> Context::getSharedContext() {
        return sharedContext;
    }

    Context::Context(const std::vector<std::tuple<const char *, bool>> &instanceExtensions,
                     const std::vector<const char *> &validationLayers,
                     const std::vector<std::tuple<const char *, bool>> &deviceExtensions, GLFWwindow *window) {

        sharedContext = std::make_shared<SharedContext>(instanceExtensions, validationLayers, deviceExtensions, window);

        //TODO: create command pools
    }

    Context::Context(std::shared_ptr<SharedContext> sharedContext) : sharedContext(std::move(sharedContext)) {
        //TODO: recreate command pools for thread safety
    }
}