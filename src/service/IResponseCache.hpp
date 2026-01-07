#pragma once

#include <string>

namespace ub {

class IResponseCache {
public:
    virtual ~IResponseCache() = default;

    virtual std::string get(const std::string& endpoint) = 0;
    virtual void put(const std::string& endpoint, const std::string& data) = 0;
    // Note: isValid is implementation specific logic for the file cache,
    // strictly speaking it might not be needed in the interface if WeatherService doesn't use it.
    // Checking WeatherService: it only uses .get() and .put().
};

}  // namespace ub
