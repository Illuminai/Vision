#ifndef VISION_PINHOLECAMERA_H
#define VISION_PINHOLECAMERA_H

#include "Camera.h"

class PinholeCamera : public Camera {
public:
    explicit PinholeCamera(Vector3d position, Vector3d rotation) : Camera(position, rotation) {}

    Ray getRay(double u, double v) override;

};


#endif //VISION_PINHOLECAMERA_H
