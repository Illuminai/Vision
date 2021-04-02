#ifndef VISION_CONTEXT_H
#define VISION_CONTEXT_H

#include <memory>
#include "vulkan/core/SharedContext.h"

namespace vulkan {
    class Context;
}

class vulkan::Context {
public:
    Context(const std::vector<std::tuple<const char *, bool>> &instanceExtensions,
            const std::vector<const char *> &validationLayers,
            const std::vector<std::tuple<const char *, bool>>& deviceExtensions,
            GLFWwindow *window);

    Context(std::shared_ptr<SharedContext> sharedContext);


    std::shared_ptr<SharedContext> getSharedContext();

private:
    std::shared_ptr<SharedContext> sharedContext;

};


#endif //VISION_CONTEXT_H
