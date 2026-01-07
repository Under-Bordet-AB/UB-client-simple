#include "FileSystem.hpp"

#include <sys/stat.h>

#include <fstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <direct.h>
#endif

namespace ub {

std::vector<char> FileSystem::readFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) return {};
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) return buffer;
    return {};
}

void FileSystem::makePath(const std::string& filePath) {
    size_t pos = filePath.find_last_of("\\/");
    if (pos != std::string::npos) {
        std::string dir = filePath.substr(0, pos);
        struct stat st;
        if (stat(dir.c_str(), &st) != 0) {
#ifdef _WIN32
            _mkdir(dir.c_str());
#else
            mkdir(dir.c_str(), 0775);
#endif
        }
    }
}

}  // namespace ub
