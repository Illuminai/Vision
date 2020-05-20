/*#include <fstream>

#include "core/render/Raytracer.h"
#include "core/camera/PinholeCamera.h"
#include "core/shape/Sphere.h"

#include <chrono>

int main() {
    auto camera = std::make_shared<PinholeCamera>(Vector3d{-10, 0, 0});

    Raytracer rt{camera};

    rt.scene.shapes.push_back(std::make_shared<Sphere>(Vector3d{1, 0, 0}, .3));

    auto pixels = rt.render_image(800, 600);

    std::ofstream file;
    file.open("test.ppm", std::ios::out | std::ios::binary);
    file << "P3\n" << 800 << " " << 600 << "\n255\n";
    for (int y = 600 - 1; y >= 0; --y) {
        for (int x = 0; x < 800; ++x) {
            file << pixels[y * 800 + x].x << " " << pixels[y * 800 + x].y << " " << pixels[y * 800 + x].z << "\n";
        }
    }
    file.close();
}*/