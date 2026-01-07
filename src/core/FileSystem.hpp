#pragma once

#include <string>
#include <vector>

namespace ub {

class FileSystem {
public:
    /// Reads the entire file into a binary vector. Returns empty on failure.
    static std::vector<char> readFile(const std::string& path);

    /// Ensures that the directory for the given file path exists.
    /// Example: given "a/b/c.txt", ensures "a/b" exists.
    static void makePath(const std::string& filePath);
};

}  // namespace ub
