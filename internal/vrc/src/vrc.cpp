#include <iostream>
#include <fstream>
#include <algorithm>

std::string getFileIdentifier(std::string filename) {
    const size_t last_slash_idx = filename.find_last_of("\\/");
    if (std::string::npos != last_slash_idx) {
        filename.erase(0, last_slash_idx + 1);
    }
    std::replace(filename.begin(), filename.end(), '.', '_');
    std::replace(filename.begin(), filename.end(), '-', '_');
    std::replace(filename.begin(), filename.end(), '/', '_');
    std::replace(filename.begin(), filename.end(), '\\', '_');
    return filename;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " {src} {dest}" << std::endl;
        return 1;
    }

    std::ifstream source{argv[1], std::ios::in | std::ios::binary};
    std::ofstream destination{argv[2], std::ios::out};

    std::string identifier = getFileIdentifier(argv[1]);

    destination << "#include <stdlib.h>" << std::endl;
    destination << "const char _vrc_resource_" << identifier << "[] = {" << std::endl;
    size_t line = 0;
    while (true) {
        char c;
        source.get(c);
        if (source.eof()) break;
        destination << "0x" << std::hex << (c & 0xff) << ", ";
        if (++line == 16) {
            destination << std::endl;
            line = 0;
        }
    }
    destination << "};" << std::endl;

    destination << "const size_t _vrc_resource_" << identifier << "_len = sizeof(_vrc_resource_" << identifier << ");";

    source.close();
    destination.close();
    return 0;
}
