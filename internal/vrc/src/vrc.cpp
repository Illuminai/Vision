#include <iostream>
#include <filesystem>
#include <fstream>

std::string getFileIdentifier(const std::string &filename) {
    std::string identifier = std::filesystem::path{filename}.filename().string();
    std::replace(identifier.begin(), identifier.end(), '.', '_');
    std::replace(identifier.begin(), identifier.end(), '-', '_');
    std::replace(identifier.begin(), identifier.end(), '/', '_');
    std::replace(identifier.begin(), identifier.end(), '\\', '_');
    return identifier;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " {src} {dest}" << std::endl;
        return 1;
    }

    std::fstream source{argv[1], std::ios::in};
    std::fstream destination{argv[2], std::ios::out};

    std::string identifier = getFileIdentifier(argv[1]);

    destination << "#include <stdlib.h>" << std::endl;
    destination << "const char _vrc_resource_" << identifier << "[] = {" << std::endl;
    size_t line = 0;
    while (!source.eof()) {
        char c;
        source.get(c);
        destination << "0x" << std::hex << (c & 0xff) << ", ";
        if (++line == 10) {
            destination << std::endl;
            line = 0;
        }
    }
    destination << "};" << std::endl << std::endl;

    destination << "const size_t _vrc_resource_" << identifier << "_len = sizeof(_vrc_resource_" << identifier << ");";

    source.close();
    destination.close();
    return 0;
}
