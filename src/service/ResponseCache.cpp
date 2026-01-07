#include "ResponseCache.hpp"

#include <sys/stat.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "core/FileSystem.hpp"
#include "core/Logger.hpp"

namespace ub {

ResponseCache::ResponseCache(const Config& config) : config_(config) {
    FileSystem::makePath(config_.cache_directory + "/");
}

std::string ResponseCache::getCacheFileName(const std::string& endpoint) {
    // Very simple sanitization
    std::string safe_name = endpoint;
    for (char& c : safe_name) {
        if (c == '/' || c == '?' || c == '&' || c == '=') c = '_';
    }
    return config_.cache_directory + "/" + safe_name;
}

std::string ResponseCache::detectExtension(const std::string& data) {
    if (data.size() >= 4) {
        if (data.substr(0, 4) == "\x89PNG") return ".png";
        if (data.substr(0, 4) == "GIF8") return ".gif";
        if (data.size() >= 2 && data.substr(0, 2) == "\xff\xd8") return ".jpg";
    }
    return ".json";
}

bool ResponseCache::isValid(const std::string& filepath) {
    struct stat result;
    if (stat(filepath.c_str(), &result) != 0) {
        return false;
    }

    const std::time_t mtime = result.st_mtime;
    const std::time_t now = std::time(nullptr);

    struct tm tm_now;
#ifdef _WIN32
    localtime_s(&tm_now, &now);
#else
    localtime_r(&now, &tm_now);
#endif
    const int current_minute = tm_now.tm_min;

    const int boundary_minute =
        (current_minute / config_.cache_interval_minutes) * config_.cache_interval_minutes;

    struct tm tm_boundary = tm_now;
    tm_boundary.tm_min = boundary_minute;
    tm_boundary.tm_sec = 0;

    const std::time_t boundary_time = mktime(&tm_boundary);

    return mtime >= boundary_time;
}

std::string ResponseCache::get(const std::string& endpoint) {
    const std::string base_path = getCacheFileName(endpoint);
    std::vector<std::string> extensions = {".json", ".png", ".gif", ".jpg"};

    for (const auto& ext : extensions) {
        std::string path = base_path + ext;
        if (isValid(path)) {
            Logger::log("Cache HIT for " + endpoint + " (" + ext + ")");
            std::ifstream file(path, std::ios::binary | std::ios::ate);
            if (file) {
                std::streamsize size = file.tellg();
                std::string str(size, '\0');
                file.seekg(0);
                if (file.read(&str[0], size)) {
                    return str;
                }
            }
        }
    }

    Logger::log("Cache MISS for " + endpoint);
    return "";
}

void ResponseCache::put(const std::string& endpoint, const std::string& data) {
    const std::string base_path = getCacheFileName(endpoint);
    std::string ext = detectExtension(data);
    std::string path = base_path + ext;

    std::ofstream file(path, std::ios::binary);
    if (file) {
        file.write(data.c_str(), data.size());
        Logger::log("Cache UPDATE for " + endpoint + " as " + ext);
    } else {
        Logger::error("Failed to write cache file: " + path);
    }
}

}  // namespace ub
