#include "core/shape/Sphere.h"
#include <algorithm>

bool Sphere::intersect(Ray &ray, RayIntersection &isect) {
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * ray.direction.dot(ray.origin - position);
    double c = (ray.origin - position).dot(ray.origin - position) - radius * radius;

    double discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0) {
        return false;
    } else {
        double time1 = (-b + std::sqrt(discriminant)) / 2.0 * a;
        double time2 = (-b - std::sqrt(discriminant)) / 2.0 * a;

        if (time1 < 0 && time2 < 0) {
            return false;
        } else if (time1 < 0 || time2 < 0) {
            isect.time = std::max(time1, time2);
        } else {
            isect.time = std::min(time1, time2);
        }
        isect.normal = ray.getPoint(isect.time) - position;

        return true;
    }
}
