#include "RequestManager.hpp"

#include <iostream>

#include "Logger.hpp"
#include "http/HttpClient.hpp"
#include "https/HttpsClient.hpp"

namespace ub {

void RequestManager::handleRequest(const std::string& mode, const std::string& endpoint,
                                   const Config& cfg, ResponseCache& cache,
                                   const UserInterface& ui) {
    // Check Cache
    std::string response = cache.get(endpoint);
    if (!response.empty()) {
        ui.showResult(mode == "--surprise" ? "surprise" : mode, response);
        return;
    }

    // Fetch from Network
    std::string scheme = cfg.use_https ? "https" : "http";
    int port = cfg.use_https ? cfg.api_port_tls : cfg.api_port_tcp;

    std::string full_url = scheme + "://" + cfg.api_host + ":" + std::to_string(port) + endpoint;
    ub::Logger::log("Fetching from network: " + full_url);

    try {
        if (cfg.use_https) {
            ub::HttpsClient client;
            response = client.get(cfg.api_host, cfg.api_port_tls, endpoint);
        } else {
            ub::HttpClient client;
            response = client.get(cfg.api_host, cfg.api_port_tcp, endpoint);
        }

        if (response.empty()) {
            ui.showError("Failed to fetch data.");
            return;
        }

        // Save to Cache
        cache.put(endpoint, response);

        // Output
        ui.showResult(mode == "--surprise" ? "surprise" : mode, response);
    } catch (const std::exception& e) {
        ui.showError(std::string("Request failed: ") + e.what());
    }
}

}  // namespace ub
