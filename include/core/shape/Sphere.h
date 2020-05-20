#ifndef VISION_SPHERE_H
#define VISION_SPHERE_H

#include "core/shape/Shape.h"

class Sphere : public Shape {
public:
    double radius;

    Sphere(Vector3d position, double radius) : Shape(position), radius(radius) {}

    bool intersect(Ray &ray, RayIntersection &isect) override;
};


#endif //VISION_SPHERE_H
