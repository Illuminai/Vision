#ifndef VISION_BASEWINDOW_H
#define VISION_BASEWINDOW_H

#include <GLFW/glfw3.h>

namespace windowing {
    class BaseWindow;
}


class windowing::BaseWindow {
public:
    BaseWindow(const std::vector<std::tuple<int, int>> &windowHints) {
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, false);
        for (auto hint : windowHints) {
            glfwWindowHint(std::get<0>(hint), std::get<1>(hint));
        }
        window = glfwCreateWindow(1920, 1080, "Vulkan", nullptr, nullptr);
    }

    virtual void onWindowRender() = 0;

    void renderWindow() {
        if (firstRender) {
            glfwShowWindow(window);
            firstRender = false;
        }
        onWindowRender();
    }

    GLFWwindow *getWindow() {
        return window;
    }

protected:
    GLFWwindow *window;

    bool firstRender{true};

};


#endif //VISION_BASEWINDOW_H
