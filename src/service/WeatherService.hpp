#pragma once

#include <string>
#include <vector>

#include "core/ConfigManager.hpp"
#include "network/INetworkFactory.hpp"
#include "service/IResponseCache.hpp"

namespace ub {

class WeatherService {
public:
    WeatherService(const Config& cfg, INetworkFactory& networkFactory, IResponseCache& cache);

    std::string getCities();
    std::string getLocation(const std::string& city);
    std::string getWeather(const std::string& lat, const std::string& lon);
    std::vector<char> getSurprise();

private:
    std::vector<char> fetchFromNetwork(const std::string& endpoint);

    const Config& cfg_;
    INetworkFactory& networkFactory_;
    IResponseCache& cache_;
};

}  // namespace ub
