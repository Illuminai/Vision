#include "windowing/WindowManager.h"

namespace windowing {

    std::shared_ptr<WindowManager> WindowManager::instance = nullptr;

    WindowManager::WindowManager() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
    }

    WindowManager::~WindowManager() {
        glfwTerminate();
    }

    std::shared_ptr<WindowManager> WindowManager::getInstance() {
        if (instance == nullptr) {
            struct MakeSharedEnabler : public WindowManager {
            };
            instance = std::make_shared<MakeSharedEnabler>();
        }
        return instance;
    }

    void WindowManager::addWindow(const std::shared_ptr<BaseWindow> &window) {
        windows.push_back(window);
    }

    void WindowManager::startManager() {
        if (!running) {
            running = true;
            loopManager();
        }
    }

    void WindowManager::stopManager() {
        if (running) {
            running = false;
        }
    }

    void WindowManager::loopManager() {
        while (running && (!windows.empty())) {
            //ImGuiWaitEvents update?
            //glfwWaitEvents();
            glfwPollEvents();

            windows.erase(std::remove_if(windows.begin(), windows.end(), [](const std::shared_ptr<BaseWindow> &window) {
                return glfwWindowShouldClose(window->getWindow());
            }), windows.end());

            std::vector<std::shared_ptr<BaseWindow>>::size_type size = windows.size();
            for (std::vector<std::shared_ptr<BaseWindow>>::size_type i = 0; i < size; ++i) {
                glfwMakeContextCurrent(windows[i]->getWindow());
                windows[i]->renderWindow();
            }
        }
    }

}