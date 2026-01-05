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

    // Timeouts (seconds)
    int connect_timeout = 10;
    int read_timeout = 30;

    // Cache
    int cache_interval_minutes = 15;
    std::string cache_directory = "./cache";

    // Logging
    std::string log_file = "app.log";
    bool log_to_console = true;

    // Other
    bool mp_enabled = false;
};

class ConfigManager {
public:
    /// Load configuration from file. Exits on failure.
    static Config load(const std::string& path);
};

}  // namespace ub
