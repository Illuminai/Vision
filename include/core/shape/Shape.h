#ifndef VISION_SHAPE_H
#define VISION_SHAPE_H

#include "core/render/RayIntersection.h"

class Shape {
public:
    Vector3d position;

    explicit Shape(Vector3d position) : position(position) {}

    virtual bool intersect(Ray &ray, RayIntersection &isect) = 0;
};


#endif //VISION_SHAPE_H
