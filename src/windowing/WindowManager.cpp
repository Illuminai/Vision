#include "windowing/WindowManager.h"

#include <GLFW/glfw3.h>
#include <stdexcept>

WindowManager::WindowManager() {
    if(!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

WindowManager::~WindowManager() {
    glfwTerminate();
}

void WindowManager::stopManager() {
    if(running) {

    }
}
