#include "WeatherService.hpp"

#include <iostream>

#include "core/Logger.hpp"
#include "core/Utils.hpp"

using ub_client::core::Utils;

namespace ub {

WeatherService::WeatherService(const Config& cfg, INetworkFactory& networkFactory,
                               IResponseCache& cache)
    : cfg_(cfg), networkFactory_(networkFactory), cache_(cache) {}

std::string WeatherService::getCities() {
    auto v = fetchFromNetwork("/GetCities");
    return std::string(v.begin(), v.end());
}

std::string WeatherService::getLocation(const std::string& city) {
    auto v = fetchFromNetwork("/GetLocation?name=" + Utils::urlEncode(city));
    return std::string(v.begin(), v.end());
}

std::string WeatherService::getWeather(const std::string& lat, const std::string& lon) {
    auto v = fetchFromNetwork("/GetWeather?lat=" + Utils::urlEncode(lat) +
                              "&lon=" + Utils::urlEncode(lon));
    return std::string(v.begin(), v.end());
}

std::vector<char> WeatherService::getSurprise() {
    // Note: Surprise might not be cached in the same way if we want fresh surprises,
    // but the current logic caches everything by endpoint.
    return fetchFromNetwork("/GetSurprise");
}

std::vector<char> WeatherService::fetchFromNetwork(const std::string& endpoint) {
    // 1. Check Cache
    std::string response = cache_.get(endpoint);
    if (!response.empty()) {
        Logger::log("Cache hit for: " + endpoint);
        return std::vector<char>(response.begin(), response.end());
    }

    // 2. Fetch from Network
    std::string scheme = cfg_.use_https ? "https" : "http";
    int port = cfg_.use_https ? cfg_.api_port_tls : cfg_.api_port_tcp;
    std::string full_url = scheme + "://" + cfg_.api_host + ":" + std::to_string(port) + endpoint;
    Logger::log("Fetching from network: " + full_url);

    // Use Factory

    // Simple retry mechanism
    Response result;
    for (int i = 0; i < cfg_.connection_retries; ++i) {
        auto client = networkFactory_.createClient();
        if (!client) {
            Logger::error("Connection failed. Retry " + std::to_string(i + 1) + "/" +
                          std::to_string(cfg_.connection_retries));
            continue;
        }

        result = client->get(cfg_.api_host, endpoint);
        if (result.status_code != 0) {
            break;
        }
        Logger::error("Empty response / Network Error. Retry " + std::to_string(i + 1) + "/" +
                      std::to_string(cfg_.connection_retries));
    }

    if (result.status_code == 0) {
        Logger::error("Failed to fetch data after " + std::to_string(cfg_.connection_retries) +
                      " attempts.");
        return {};
    }

    // 3. Save to Cache ONLY if 200 OK
    if (result.isSuccess()) {
        if (!result.body.empty()) {
            std::string bodyStr(result.body.begin(), result.body.end());
            cache_.put(endpoint, bodyStr);
            return result.body;
        } else {
            Logger::log("Received empty body with 200 OK");
            // Decide: do we cache empty body? Probably not useful.
            return {};
        }
    } else {
        Logger::error("HTTP Error " + std::to_string(result.status_code) + " for " + endpoint);
        return {};
    }
}

}  // namespace ub
