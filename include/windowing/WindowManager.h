#ifndef VISION_WINDOWMANAGER_H
#define VISION_WINDOWMANAGER_H

#include <vector>
#include <memory>
#include <stdexcept>
#include "windowing/BaseWindow.h"

namespace windowing {
    class WindowManager;
}

class windowing::WindowManager {
public:
    ~WindowManager();

    void startManager();

    void stopManager();

    void loopManager();

    void addWindow(const std::shared_ptr<BaseWindow> &window);

    static std::shared_ptr<WindowManager> getInstance();

private:
    WindowManager();

    bool running{false};

    std::vector<std::shared_ptr<BaseWindow>> windows;

    static std::shared_ptr<WindowManager> instance;

};


#endif //VISION_WINDOWMANAGER_H
