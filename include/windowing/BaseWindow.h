#ifndef VISION_BASEWINDOW_H
#define VISION_BASEWINDOW_H

namespace windowing {
    class BaseWindow;
}


class windowing::BaseWindow {
public:

    virtual void onRender() = 0;


protected:

};


#endif //VISION_BASEWINDOW_H
