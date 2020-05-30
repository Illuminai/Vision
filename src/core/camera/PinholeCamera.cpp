#include "core/camera/PinholeCamera.h"

Ray PinholeCamera::getRay(double u, double v) {
    Vector3d direction = Vector3d{10, 0, 0} +
                         Vector3d{0, 4, 0} * u +
                         Vector3d{0, 0, -3} * v;
    return Ray{position, direction};
}
