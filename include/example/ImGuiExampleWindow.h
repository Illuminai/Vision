#ifndef VISION_IMGUIEXAMPLEWINDOW_H
#define VISION_IMGUIEXAMPLEWINDOW_H

#include "windowing/ImGuiWindow.h"

class ImGuiExampleWindow : public windowing::ImGuiWindow {
public:
    ImGuiExampleWindow();

    void onImGuiFrameRender() override;

    void toggleButton(const char* str_id, bool* v);

    bool test{false};
};


#endif //VISION_IMGUIEXAMPLEWINDOW_H
