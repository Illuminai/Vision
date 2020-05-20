#ifndef VISION_RAYTRACER_H
#define VISION_RAYTRACER_H

#include "core/scene/Scene.h"
#include "core/camera/Camera.h"

#include <utility>
#include <vector>

class Raytracer {
public:
    std::shared_ptr<Camera> camera;
    Scene scene;

    explicit Raytracer(std::shared_ptr<Camera> camera) : camera(std::move(camera)) {}

    std::vector<Vector3d> render_image(int width, int height);

    Vector3d get_color(Ray &ray);

    bool get_intersection(Ray &ray, RayIntersection &isec);
};

#endif //VISION_RAYTRACER_H
