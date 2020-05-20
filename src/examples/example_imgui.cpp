/*#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "core/shape/Sphere.h"

GLFWwindow *window;
ImGuiIO *imGuiIo;

#include "core/render/Raytracer.h"
#include "core/camera/PinholeCamera.h"

Raytracer rt{std::make_shared<PinholeCamera>(Vector3d{-10, 0, 0})};


void draw_screen();

void glfw_error_callback(int error, const char *description) {
    std::cout << "GLFW Error " << error << ": " << description << std::endl;
}

void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    draw_screen();
}

static int listbox_item_current = 1;
float color[] = {.3f, .4f, .5f};

bool fixedLayout = true;

void draw_screen() {

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::DockSpaceOverViewport();


    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(window, true);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window")) {
            if (ImGui::MenuItem("Fixed Layout", nullptr, &fixedLayout)) {

            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    {
        ImGui::Begin("Scene");

        //ImGui::ListBox("", &listbox_item_current, nullptr, IM_ARRAYSIZE(rt.scene.shapes), 7);

        ImGui::ListBoxHeader("");

        for (auto shape : rt.scene.shapes) {
            if (ImGui::Selectable("Shape", true)) {
                std::cout << "Test" << std::endl;
            }
        }

        ImGui::ListBoxFooter();

        ImGui::End();
    }

    {
        ImGui::Begin("Properties");
        //ImGui::BulletText("Selected: %s", listbox_items[listbox_item_current]);
        ImGui::ColorEdit3("Color", color, 0);
        ImGui::End();
    }

    {
        ImGui::Begin("Node Editor");
        ImGui::Text("This is the node editor");
        ImGui::End();
    }

    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Preview");
        ImGui::PopStyleVar(1);

        ImGui::End();
    }


    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    if (imGuiIo->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }


    glfwSwapBuffers(window);
}

int main() {

    rt.scene.shapes.push_back(std::make_shared<Sphere>(Vector3d{1, 1, 1}, .3));
    rt.scene.shapes.push_back(std::make_shared<Sphere>(Vector3d{1, 2, 1}, .3));

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    window = glfwCreateWindow(1280, 720, "Vision", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
    glfwSetKeyCallback(window, glfw_key_callback);

    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));


    ImGui::CreateContext();
    imGuiIo = &ImGui::GetIO();
    imGuiIo->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    imGuiIo->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    imGuiIo->ConfigWindowsResizeFromEdges = true;


    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while (!glfwWindowShouldClose(window)) {
        draw_screen();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
*/