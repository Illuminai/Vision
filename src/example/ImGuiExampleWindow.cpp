#include "example/ImGuiExampleWindow.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <cmrc/cmrc.hpp>

#include "IconsFontAwesome5.h"
CMRC_DECLARE(icons);

ImGuiExampleWindow::ImGuiExampleWindow() {
    auto fs = cmrc::icons::get_filesystem();
    auto img = fs.open("icons/sphere.png");

    GLFWimage images[1];
    images[0].pixels = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(img.begin()), img.size(),
                                             &images[0].width, &images[0].height, nullptr, 4);
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    constexpr auto ColorFromBytes = [](uint8_t r, uint8_t g, uint8_t b) {
        return ImVec4((float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f, 1.0f);
    };

    auto &style = ImGui::GetStyle();
    ImVec4 *colors = style.Colors;

    const ImVec4 bgColor = ColorFromBytes(37, 37, 38);
    const ImVec4 lightBgColor = ColorFromBytes(82, 82, 85);
    const ImVec4 veryLightBgColor = ColorFromBytes(90, 90, 95);

    const ImVec4 panelColor = ColorFromBytes(51, 51, 55);
    const ImVec4 panelHoverColor = ColorFromBytes(29, 151, 236);
    const ImVec4 panelActiveColor = ColorFromBytes(0, 119, 200);

    const ImVec4 textColor = ColorFromBytes(255, 255, 255);
    const ImVec4 textDisabledColor = ColorFromBytes(151, 151, 151);
    const ImVec4 borderColor = ColorFromBytes(78, 78, 78);

    colors[ImGuiCol_Text] = textColor;
    colors[ImGuiCol_TextDisabled] = textDisabledColor;
    colors[ImGuiCol_TextSelectedBg] = panelActiveColor;
    colors[ImGuiCol_WindowBg] = bgColor;
    colors[ImGuiCol_ChildBg] = bgColor;
    colors[ImGuiCol_PopupBg] = bgColor;
    colors[ImGuiCol_Border] = borderColor;
    colors[ImGuiCol_BorderShadow] = borderColor;
    colors[ImGuiCol_FrameBg] = panelColor;
    colors[ImGuiCol_FrameBgHovered] = panelHoverColor;
    colors[ImGuiCol_FrameBgActive] = panelActiveColor;
    colors[ImGuiCol_TitleBg] = bgColor;
    colors[ImGuiCol_TitleBgActive] = bgColor;
    colors[ImGuiCol_TitleBgCollapsed] = bgColor;
    colors[ImGuiCol_MenuBarBg] = panelColor;
    colors[ImGuiCol_ScrollbarBg] = panelColor;
    colors[ImGuiCol_ScrollbarGrab] = lightBgColor;
    colors[ImGuiCol_ScrollbarGrabHovered] = veryLightBgColor;
    colors[ImGuiCol_ScrollbarGrabActive] = veryLightBgColor;
    colors[ImGuiCol_CheckMark] = panelActiveColor;
    colors[ImGuiCol_SliderGrab] = panelHoverColor;
    colors[ImGuiCol_SliderGrabActive] = panelActiveColor;
    colors[ImGuiCol_Button] = panelColor;
    colors[ImGuiCol_ButtonHovered] = panelHoverColor;
    colors[ImGuiCol_ButtonActive] = panelHoverColor;
    colors[ImGuiCol_Header] = panelColor;
    colors[ImGuiCol_HeaderHovered] = panelHoverColor;
    colors[ImGuiCol_HeaderActive] = panelActiveColor;
    colors[ImGuiCol_Separator] = borderColor;
    colors[ImGuiCol_SeparatorHovered] = borderColor;
    colors[ImGuiCol_SeparatorActive] = borderColor;
    colors[ImGuiCol_ResizeGrip] = bgColor;
    colors[ImGuiCol_ResizeGripHovered] = panelColor;
    colors[ImGuiCol_ResizeGripActive] = lightBgColor;
    colors[ImGuiCol_PlotLines] = panelActiveColor;
    colors[ImGuiCol_PlotLinesHovered] = panelHoverColor;
    colors[ImGuiCol_PlotHistogram] = panelActiveColor;
    colors[ImGuiCol_PlotHistogramHovered] = panelHoverColor;
    colors[ImGuiCol_ModalWindowDimBg] = bgColor;
    colors[ImGuiCol_DragDropTarget] = bgColor;
    colors[ImGuiCol_NavHighlight] = bgColor;
    colors[ImGuiCol_DockingPreview] = panelActiveColor;
    colors[ImGuiCol_Tab] = bgColor;
    colors[ImGuiCol_TabActive] = panelActiveColor;
    colors[ImGuiCol_TabUnfocused] = bgColor;
    colors[ImGuiCol_TabUnfocusedActive] = panelActiveColor;
    colors[ImGuiCol_TabHovered] = panelHoverColor;

    colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.32f, 0.32f, 0.35f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.28f, 0.31f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.58f);


    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(8, 3);
    style.CellPadding = ImVec2(4, 2);
    style.ItemSpacing = ImVec2(8, 4);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 21.0f;
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 10.0f;

    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 0;
    style.TabBorderSize = 0;

    style.WindowRounding = 5;
    style.ChildRounding = 0;
    style.FrameRounding = 3;
    style.PopupRounding = 5;
    style.ScrollbarRounding = 2;
    style.GrabRounding = 2;
    style.LogSliderDeadzone = 4;
    style.TabRounding = 3;

    style.WindowTitleAlign = ImVec2(0, 0.5);
    style.ButtonTextAlign = ImVec2(0.5, 0.5);

    context = ImPlot::CreateContext();
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
        ImGuiID dockCenter = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Up, 0.80f, nullptr, &mainDock);
        ImGuiID dockRight = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Right, 0.20f, nullptr, &mainDock);
        ImGuiID dockBottom = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Down, 0.20f, nullptr, &dockCenter);

        ImGui::DockBuilderDockWindow("Center", dockCenter);
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
    /*if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, ImGui::GetFrameHeight(),
                                    statusBarFlags)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::Text("Happy secondary menu bar");
            ImGui::SameLine();
            ImGui::Button("Hello");
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }*/

    ImGui::PopStyleVar();

    {

        ImGui::Begin("Right", nullptr, 0);
        ImGui::Text("Properties");

        ImGui::Text(ICON_FA_PENCIL_ALT "  Paint");
        ImGui::End();
    }

    {
        ImGuiWindowClass windowClass{};
        windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
        ImGui::SetNextWindowClass(&windowClass);

        ImGui::Begin("Center", nullptr, 0);
        ImGui::Text("Top Pane");
        ImGui::End();
    }

    {
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoNav;
        ImGui::Begin("Bottom", nullptr, flags);
        ImGui::Text("Bottom Pane");
        ImGui::End();
    }


    //ImPlot::ShowDemoWindow();
    ImGui::ShowDemoWindow();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    if (ImGui::BeginViewportSideBar("##StatusBar", viewport, ImGuiDir_Down, ImGui::GetFrameHeight(), statusBarFlags)) {
        if (ImGui::BeginMenuBar()) {
            //ImGui::Text("This is the status bar");
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }
    ImGui::PopStyleVar();
}

ImGuiExampleWindow::~ImGuiExampleWindow() {
    ImPlot::DestroyContext(context);
}
