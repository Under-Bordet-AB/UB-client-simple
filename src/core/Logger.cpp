#include "Logger.hpp"

#include <ctime>
#include <fstream>
#include <iostream>

#include "core/FileSystem.hpp"

namespace ub {

std::string Logger::log_file_ = "logs/app.log";  // NOLINT(cert-err58-cpp)
bool Logger::console_ = true;

void Logger::init(const std::string& path, bool console) {
    log_file_ = path;
    console_ = console;
}

void Logger::log(const std::string& message) {
    const std::time_t now = std::time(nullptr);
    char buf[80];
    struct tm tm_now;
#ifdef _WIN32
    localtime_s(&tm_now, &now);
#else
    localtime_r(&now, &tm_now);
#endif
    (void)std::strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S] ", &tm_now);

    const std::string entry = std::string(buf) + "INFO: " + message;

    if (console_) {
        std::cout << entry << "\n";
    }

    FileSystem::makePath(log_file_);
    std::ofstream file(log_file_, std::ios::app);
    if (file) {
        file << entry << "\n";
    }
}

void Logger::error(const std::string& message) {
    const std::time_t now = std::time(nullptr);
    char buf[80];
    struct tm tm_now;
#ifdef _WIN32
    localtime_s(&tm_now, &now);
#else
    localtime_r(&now, &tm_now);
#endif
    (void)std::strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S] ", &tm_now);

    const std::string entry = std::string(buf) + "ERROR: " + message;

    if (console_) {
        std::cerr << entry << "\n";
    }

    FileSystem::makePath(log_file_);
    std::ofstream file(log_file_, std::ios::app);
    if (file) {
        file << entry << "\n";
    }
}

}  // namespace ub
