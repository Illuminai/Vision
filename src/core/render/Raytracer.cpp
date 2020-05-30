#include "core/render/Raytracer.h"

#include <iostream>
#include <fstream>

std::vector<Vector3d> Raytracer::renderImage(int width, int height) {
    std::vector<Vector3d> pixels{};

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double u = (x - static_cast<double>(width) / 2) / static_cast<double>(width);
            double v = (y - static_cast<double>(height) / 2) / static_cast<double>(height);
            Ray ray = camera->getRay(u, v);

            Vector3d color = getColor(ray);

            pixels.push_back(color);
        }
    }

    return pixels;
}

Vector3d Raytracer::getColor(Ray &ray) {
    RayIntersection isec;
    if (!getIntersection(ray, isec)) {
        // No intersection, display background
        return Vector3d(0.0, 255, 0.0);
    }
    return Vector3d(255, 0.0, 255);
}

bool Raytracer::getIntersection(Ray &ray, RayIntersection &isec) {
    for (const auto &shape_ptr : scene.shapes) {
        if (shape_ptr->intersect(ray, isec)) {
            return true;
        }
    }
    return false;
}
