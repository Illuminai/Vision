#include <iostream>
#include <vrc.h>
#include "frontend/PlatformWindow.h"

int main() {

    // VRC example
    auto res = VRC_LOAD_RESOURCE(rt_comp_spv);
    std::cout << res.data() << std::endl;

/*
    PlatformWindow window{};
    window.initWindow();
    window.loop();
    window.destroyWindow();
*/

    return 0;
}