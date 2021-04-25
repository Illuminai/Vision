#include "example/ImGuiExampleWindow.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <cmrc/cmrc.hpp>

CMRC_DECLARE(icons);

ImGuiExampleWindow::ImGuiExampleWindow() {
    auto fs = cmrc::icons::get_filesystem();
    auto img = fs.open("icons/sphere.png");

    GLFWimage images[1];
    images[0].pixels = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(img.begin()), img.size(),
                                             &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);
}

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


    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Settings")) {
            }
            if (ImGui::MenuItem("Exit")) {
                exit(0);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }


    ImGuiWindowFlags statusBarFlags = ImGuiWindowFlags_NoScrollbar
                                      | ImGuiWindowFlags_NoSavedSettings
                                      | ImGuiWindowFlags_MenuBar;
    if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, ImGui::GetFrameHeight(),
                                    statusBarFlags)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::Text("Happy secondary menu bar");
            ImGui::SameLine();
            ImGui::Button("Hello");
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }

    ImGui::PopStyleVar();

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

    ImGui::ShowDemoWindow();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    if (ImGui::BeginViewportSideBar("##StatusBar", viewport, ImGuiDir_Down, ImGui::GetFrameHeight(), statusBarFlags)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::Text("Happy status bar");
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }
    ImGui::PopStyleVar();
}
