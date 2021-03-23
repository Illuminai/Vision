#include <vector>
#include <cmrc/cmrc.hpp>
#include <iostream>

CMRC_DECLARE(shaders);

int main() {
    auto fs = cmrc::shaders::get_filesystem();
    auto f = fs.open("test.frag");
    std::cout << std::string(f.begin(), f.end()) << std::endl;

    return 0;
}