#pragma once

#include <string>
#include <vector>

#include "core/ConfigManager.hpp"
#include "service/IResponseCache.hpp"

namespace ub {

class ResponseCache : public IResponseCache {
public:
    explicit ResponseCache(const Config& config);

    virtual ~ResponseCache() = default;

    // Returns empty string if no valid cache found
    std::string get(const std::string& endpoint) override;

    void put(const std::string& endpoint, const std::string& data) override;

    // Returns true if cache for this specific file is valid based on 15-min rule
    bool isValid(const std::string& filepath);

private:
    std::string getCacheFileName(const std::string& endpoint);

    Config config_;

    std::string detectExtension(const std::string& data);
};

}  // namespace ub
