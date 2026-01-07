#pragma once

#include <fstream>
#include <iostream>
#include <string>

namespace ub {

struct Config {
    // Network
    std::string api_host = "stockholm1.onvo.se";
    int api_port_tcp = 10480;
    int api_port_tls = 10443;
    bool use_https = true;
    std::string ca_cert_path = "/etc/ssl/certs/ca-certificates.crt";

    // Timeouts (seconds)
    int connect_timeout = 10;
    int read_timeout = 30;

    // Cache
    int cache_interval_minutes = 15;
    std::string cache_directory = "./cache";

    // Logging
    std::string log_file = "logs/app.log";
    bool log_to_console = true;

    // Other
    bool mp_enabled = false;

    // New Fields
    std::string user_agent = "UB-WeatherClient/3.0";
    std::string default_location = "Stockholm";
    std::string default_lat = "59.32";
    std::string default_lon = "18.06";
    int connection_retries = 3;
    std::string ui_banner_text = "UB-Weather Client Interface (V3)";
};

struct ConfigResult {
    Config config;
    bool success;
    std::string error_message;
};

class ConfigManager {
public:
    /// Load configuration from file. Returns a result object.
    static ConfigResult load(const std::string& path);
};

}  // namespace ub
