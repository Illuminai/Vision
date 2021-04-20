#include "example/ImGuiExampleWindow.h"

void ImGuiExampleWindow::onImGuiFrameRender() {

    ImGuiViewport *viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    auto dockWindowFlags = ImGuiWindowFlags_NoDocking
                           | ImGuiWindowFlags_NoCollapse
                           | ImGuiWindowFlags_NoResize
                           | ImGuiWindowFlags_NoMove
                           | ImGuiWindowFlags_NoBringToFrontOnFocus
                           | ImGuiWindowFlags_NoBackground
                           | ImGuiWindowFlags_NoTitleBar;


    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Dock", nullptr, dockWindowFlags);
    ImGui::PopStyleVar(3);

    if (ImGui::DockBuilderGetNode(ImGui::GetID("MainDockspace")) == nullptr) {
        ImGuiID dockspaceId = ImGui::GetID("MainDockspace");
        ImGui::DockBuilderRemoveNode(dockspaceId);
        ImGui::DockBuilderAddNode(dockspaceId);

        ImGuiID mainDock = dockspaceId;
        ImGuiID dockLeft = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Left, 0.20f, nullptr, &mainDock);
        ImGuiID dockTop = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Up, 0.80f, nullptr, &mainDock);
        ImGuiID dockRight = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Right, 0.20f, nullptr, &mainDock);
        ImGuiID dockBottom = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Down, 0.20f, nullptr, &mainDock);

        ImGui::DockBuilderDockWindow("Left", dockLeft);
        ImGui::DockBuilderDockWindow("Top", dockTop);
        ImGui::DockBuilderDockWindow("Right", dockRight);
        ImGui::DockBuilderDockWindow("Bottom", dockBottom);

        ImGui::DockBuilderFinish(dockspaceId);
    }
    auto dockFlags = ImGuiDockNodeFlags_PassthruCentralNode
                     | ImGuiDockNodeFlags_NoWindowMenuButton
                     | ImGuiDockNodeFlags_NoCloseButton;

    ImGui::DockSpace(ImGui::GetID("MainDockspace"), ImVec2(0, 0), dockFlags);
    ImGui::End();


    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                exit(0);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGuiWindowFlags statusBarFlags = ImGuiWindowFlags_NoScrollbar
                                      | ImGuiWindowFlags_NoSavedSettings
                                      | ImGuiWindowFlags_MenuBar;
    float height = ImGui::GetFrameHeight();
    if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, statusBarFlags)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::Text("Happy secondary menu bar");
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }

    ImGui::Begin("Left", nullptr, 0);
    ImGui::Text("Left Pane");
    ImGui::End();

    ImGui::Begin("Right", nullptr, 0);
    ImGui::Text("Right Pane");
    ImGui::End();

    ImGui::Begin("Top", nullptr, 0);
    ImGui::Text("Top Pane");
    ImGui::End();

    ImGui::Begin("Bottom", nullptr, 0);
    ImGui::Text("Bottom Pane");
    ImGui::End();


    //ImVec2 pos = ImGui::GetIO().MousePos;
    //ImDrawList *draw_list = ImGui::GetOverlayDrawList();
    //draw_list->AddCircleFilled(pos, 130, ImColor(255, 255, 255));

    //draw_list->AddCircleFilled(pos, 100, ImColor(0,0,0));

    ImGui::ShowDemoWindow();

    if (ImGui::BeginViewportSideBar("##StatusBar", viewport, ImGuiDir_Down, height, statusBarFlags)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::Text("Happy status bar");
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }
}
