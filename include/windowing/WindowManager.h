#ifndef VISION_WINDOWMANAGER_H
#define VISION_WINDOWMANAGER_H

#include <vector>

class WindowManager {
public:
    WindowManager();

    ~WindowManager();

    void startManager();

    void stopManager();

private:
    bool running;


};


#endif //VISION_WINDOWMANAGER_H
