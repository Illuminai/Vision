#ifndef VISION_IMGUIEXAMPLEWINDOW_H
#define VISION_IMGUIEXAMPLEWINDOW_H

#include "windowing/ImGuiWindow.h"

class ImGuiExampleWindow : public windowing::ImGuiWindow {
public:
    ImGuiExampleWindow();

    void onImGuiFrameRender() override;

};


#endif //VISION_IMGUIEXAMPLEWINDOW_H
