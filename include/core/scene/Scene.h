#ifndef VISION_SCENE_H
#define VISION_SCENE_H

#include <memory>
#include <vector>

#include "core/shape/Shape.h"

class Scene {
public:
    std::vector<std::shared_ptr<Shape>> shapes;
};

#endif //VISION_SCENE_H
