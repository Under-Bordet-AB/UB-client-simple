#include "ResponseCache.hpp"

#include <sys/stat.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Logger.hpp"

namespace ub {

ResponseCache::ResponseCache(const Config& config) : config_(config) {
    // Ensure cache directory exists (rough implementation using POSIX mkdir)
    // Mode 0755: rwxr-xr-x
    mkdir(config_.cache_directory.c_str(), 0755);
}

std::string ResponseCache::getCacheFileName(const std::string& endpoint) {
    // Very simple sanitization
    std::string safe_name = endpoint;
    for (char& c : safe_name) {
        if (c == '/' || c == '?' || c == '&' || c == '=') c = '_';
    }
    return config_.cache_directory + "/" + safe_name + ".json";
}

bool ResponseCache::isValid(const std::string& filepath) {
    struct stat result;
    if (stat(filepath.c_str(), &result) != 0) {
        return false;
    }

    const std::time_t mtime = result.st_mtime;
    const std::time_t now = std::time(nullptr);

    struct tm* tm_now = std::localtime(&now);
    const int current_minute = tm_now->tm_min;

    // Calculate last boundary
    // e.g. if now is 10:23, boundary is 10:15
    // if now is 10:05, boundary is 10:00
    const int boundary_minute =
        (current_minute / config_.cache_interval_minutes) * config_.cache_interval_minutes;

    struct tm tm_boundary = *tm_now;
    tm_boundary.tm_min = boundary_minute;
    tm_boundary.tm_sec = 0;

    const std::time_t boundary_time = mktime(&tm_boundary);

    // If cache file is older than the last boundary, it's invalid
    return mtime >= boundary_time;
}

std::string ResponseCache::get(const std::string& endpoint) {
    const std::string path = getCacheFileName(endpoint);

    if (isValid(path)) {
        Logger::log("Cache ALLZ for " + endpoint);
        const std::ifstream file(path);
        if (file) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }
    }

    Logger::log("Cache MISS for " + endpoint);
    return "";
}

void ResponseCache::put(const std::string& endpoint, const std::string& data) {
    const std::string path = getCacheFileName(endpoint);
    std::ofstream file(path);
    if (file) {
        file << data;
        Logger::log("Cache UPDATE for " + endpoint);
    } else {
        Logger::error("Failed to write cache file: " + path);
    }
}

}  // namespace ub
