#ifndef VISION_BASEWINDOW_H
#define VISION_BASEWINDOW_H

#include <GLFW/glfw3.h>

namespace windowing {
    class BaseWindow;
}


class windowing::BaseWindow {
public:

    virtual void renderWindow() = 0;

    GLFWwindow *getWindow() {
        return window;
    }

protected:
    GLFWwindow *window;

};


#endif //VISION_BASEWINDOW_H
