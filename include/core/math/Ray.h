#ifndef VISION_RAY_H
#define VISION_RAY_H

#include "Vector3.h"

class Ray {
public:
    Vector3d origin;
    Vector3d direction;

    Ray() = default;

    Ray(const Vector3d &origin, const Vector3d &direction) : origin(origin), direction(direction) {}

    Vector3d point_at_time(double time) const {
        return origin + direction * time;
    }

    friend std::ostream &operator<<(std::ostream &os, const Ray &ray) {
        os << "Ray[origin=" << ray.origin << ", direction=" << ray.direction << "]";
        return os;
    }
};

#endif //VISION_RAY_H
