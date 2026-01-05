#pragma once

#include <string>
#include <vector>

#include "ConfigManager.hpp"

namespace ub {

class ResponseCache {
public:
    explicit ResponseCache(const Config &config);

    // Returns empty string if no valid cache found
    std::string get(const std::string &endpoint);

    void put(const std::string &endpoint, const std::string &data);

    // Returns true if cache for this specific file is valid based on 15-min rule
    bool isValid(const std::string &filepath);

private:
    std::string getCacheFileName(const std::string &endpoint);

    Config config_;
};

}  // namespace ub
