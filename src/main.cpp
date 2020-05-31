#include "frontend/PlatformWindow.h"

int main() {
    PlatformWindow window{};
    window.initWindow();
    window.loop();
    window.destroyWindow();

    return 0;
}