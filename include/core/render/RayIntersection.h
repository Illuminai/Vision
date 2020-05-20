#ifndef VISION_RAYINTERSECTION_H
#define VISION_RAYINTERSECTION_H

#include "core/math/Ray.h"


class RayIntersection {
public:
    Ray ray;
    double time;
    Vector3d normal;
    Vector3d point;



};

#endif //VISION_RAYINTERSECTION_H
