#include "core/render/Raytracer.h"

#include <iostream>
#include <fstream>

std::vector<Vector3d> Raytracer::render_image(int width, int height) {
    std::vector<Vector3d> pixels{};

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double u = (x - static_cast<double>(width) / 2) / static_cast<double>(width);
            double v = (y - static_cast<double>(height) / 2) / static_cast<double>(height);
            Ray ray = camera->get_ray(u, v);

            Vector3d color = get_color(ray);

            pixels.push_back(color);
        }
    }

    return pixels;
}

Vector3d Raytracer::get_color(Ray &ray) {
    RayIntersection isec;
    if (!get_intersection(ray, isec)) {
        // No intersection, display background
        return Vector3d(0.0, 255, 0.0);
    }
    return Vector3d(255, 0.0, 255);
}

bool Raytracer::get_intersection(Ray &ray, RayIntersection &isec) {
    for (const auto &shape_ptr : scene.shapes) {
        if (shape_ptr->intersect(ray, isec)) {
            return true;
        }
    }
    return false;
}
