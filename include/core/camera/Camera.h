#ifndef VISION_CAMERA_H
#define VISION_CAMERA_H

#include "core/math/Ray.h"
#include "core/math/Matrix.h"

class Camera {
public:
    Vector3d position;
    Vector3d rotation;
    Matrix<3, 3> rotationMatrix{std::array<double, 9>{}};

    Camera(Vector3d position, Vector3d rotation) {
        this->position = position;
        this->rotation = rotation;
        Matrix<3, 3>::getRotationMatrix('x', 25);

    }

    virtual Ray getRay(double u, double v) = 0;
};


#endif //VISION_CAMERA_H
