#ifndef UB_CLIENT_CORE_UTILS_HPP
#define UB_CLIENT_CORE_UTILS_HPP

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace ub_client {
namespace core {

class Utils {
public:
    // Trims leading and trailing whitespace from a string
    static std::string trim(const std::string& str) {
        auto start = str.begin();
        while (start != str.end() && std::isspace(*start)) {
            start++;
        }

        auto end = str.end();
        do {
            end--;
        } while (std::distance(start, end) > 0 && std::isspace(*end));

        return std::string(start, end + 1);
    }

    // Reads a file completely into a vector of chars (binary allowed)
    static std::vector<char> readFile(const std::string& path) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file) {
            return {};
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (file.read(buffer.data(), size)) {
            return buffer;
        }
        return {};
    }

    // Converts string to int, returns false if failed
    static bool toInt(const std::string& str, int& out) {
        if (str.empty()) return false;
        char* end = nullptr;
        // errno = 0; // standard usually doesn't require setting errno to 0 for strtol unless
        // checking specific errors, but good practice. simplified for brevity and strictness.
        long val = std::strtol(str.c_str(), &end, 10);

        if (end == str.c_str() || *end != '\0') {
            return false;
        }

        // Simple range check
        if (val > 2147483647L || val < -2147483648L) {
            return false;
        }

        out = static_cast<int>(val);
        return true;
    }

    static std::string urlEncode(const std::string& value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (std::string::const_iterator i = value.begin(); i != value.end(); ++i) {
            std::string::value_type c = (*i);

            // Keep alphanumeric and other accepted characters intact
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
                continue;
            }

            // Any other characters are percent-encoded
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int((unsigned char)c);
            escaped << std::nouppercase;
        }

        return escaped.str();
    }
};

}  // namespace core
}  // namespace ub_client

#endif  // UB_CLIENT_CORE_UTILS_HPP
