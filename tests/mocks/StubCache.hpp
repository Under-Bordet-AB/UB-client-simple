#pragma once

#include <map>
#include <string>

#include "service/IResponseCache.hpp"

namespace ub {

class StubCache : public IResponseCache {
public:
    std::map<std::string, std::string> memoryCache;

    StubCache() {}

    std::string get(const std::string& endpoint) override {
        if (memoryCache.find(endpoint) != memoryCache.end()) {
            return memoryCache[endpoint];
        }
        return "";
    }

    void put(const std::string& endpoint, const std::string& data) override {
        memoryCache[endpoint] = data;
    }
};

}  // namespace ub
