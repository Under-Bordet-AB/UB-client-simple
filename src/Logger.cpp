#include "Logger.hpp"

#include <ctime>
#include <fstream>
#include <iostream>

namespace ub {

std::string Logger::log_file_ = "app.log";  // NOLINT(cert-err58-cpp)
bool Logger::console_ = true;

void Logger::init(const std::string& path, bool console) {
    log_file_ = path;
    console_ = console;
}

void Logger::log(const std::string& message) {
    const std::time_t now = std::time(nullptr);
    char buf[80];
    (void)std::strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S] ", std::localtime(&now));

    const std::string entry = std::string(buf) + "INFO: " + message;

    if (console_) {
        std::cout << entry << "\n";
    }

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

    std::ofstream file(log_file_, std::ios::app);
    if (file) {
        file << entry << "\n";
    }
}

}  // namespace ub
