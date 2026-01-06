#include "Logger.hpp"

#include <sys/stat.h>
#include <sys/types.h>

#include <ctime>
#include <fstream>
#include <iostream>

namespace ub {

std::string Logger::log_file_ = "logs/app.log";  // NOLINT(cert-err58-cpp)
bool Logger::console_ = true;

void Logger::init(const std::string& path, bool console) {
    log_file_ = path;
    console_ = console;
}

static void ensure_directory_exists(const std::string& path) {
    size_t pos = path.find_last_of("\\/");
    if (pos != std::string::npos) {
        std::string dir = path.substr(0, pos);
        struct stat st;
        if (stat(dir.c_str(), &st) != 0) {
            // Directory does not exist, try to create it
            // S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH = 0775
#ifdef _WIN32
            _mkdir(dir.c_str());
#else
            mkdir(dir.c_str(), 0775);
#endif
        }
    }
}

void Logger::log(const std::string& message) {
    const std::time_t now = std::time(nullptr);
    char buf[80];
    (void)std::strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S] ", std::localtime(&now));

    const std::string entry = std::string(buf) + "INFO: " + message;

    if (console_) {
        std::cout << entry << "\n";
    }

    ensure_directory_exists(log_file_);
    std::ofstream file(log_file_, std::ios::app);
    if (file) {
        file << entry << "\n";
    }
}

void Logger::error(const std::string& message) {
    const std::time_t now = std::time(nullptr);
    char buf[80];
    (void)std::strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S] ", std::localtime(&now));

    const std::string entry = std::string(buf) + "ERROR: " + message;

    if (console_) {
        std::cerr << entry << "\n";
    }

    ensure_directory_exists(log_file_);
    std::ofstream file(log_file_, std::ios::app);
    if (file) {
        file << entry << "\n";
    }
}

}  // namespace ub
