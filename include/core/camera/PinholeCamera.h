#ifndef VISION_PINHOLECAMERA_H
#define VISION_PINHOLECAMERA_H

#include "Camera.h"

class PinholeCamera : public Camera {
public:
    explicit PinholeCamera(Vector3d position) : Camera(position) {}

    Ray get_ray(double u, double v) override;

};


#endif //VISION_PINHOLECAMERA_H
