#include "windowing/WindowManager.h"
#include "example/ImGuiExampleWindow.h"
#include "example/RaytracingExampleWindow.h"

int main() {
    auto manager = windowing::WindowManager::getInstance();

    //manager->addWindow(std::make_shared<RaytracingExampleWindow>());
    manager->addWindow(std::make_shared<ImGuiExampleWindow>());

    manager->startManager();

    return 0;
}