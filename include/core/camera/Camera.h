#ifndef VISION_CAMERA_H
#define VISION_CAMERA_H

#include "core/math/Ray.h"

class Camera {
public:
    Vector3d position;

    explicit Camera(Vector3d position) : position(position) {}

    virtual Ray get_ray(double u, double v) = 0;
};


#endif //VISION_CAMERA_H
