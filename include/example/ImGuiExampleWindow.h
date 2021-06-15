#ifndef VISION_IMGUIEXAMPLEWINDOW_H
#define VISION_IMGUIEXAMPLEWINDOW_H

#include "windowing/ImGuiWindow.h"

#include <implot.h>


class ImGuiExampleWindow : public windowing::ImGuiWindow {
public:
    ImGuiExampleWindow();

    ~ImGuiExampleWindow();

    void onImGuiFrameRender() override;


private:
    ImPlotContext *context;


};


#endif //VISION_IMGUIEXAMPLEWINDOW_H
