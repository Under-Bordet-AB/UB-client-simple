#include "ConfigManager.hpp"

#include <algorithm>
#include <cstdlib>

namespace ub {

Config ConfigManager::load(const std::string& path) {
    Config cfg;

    std::ifstream file(path);
    if (!file) {
        std::cerr << "[ConfigManager] Error: Config file not found at " << path << std::endl;
        std::cerr << "[ConfigManager] Critical error, exiting." << std::endl;
        std::exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Trim whitespace (simple version)
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;

        const size_t pos = line.find('=');
        if (pos == std::string::npos) continue;

        const std::string key = line.substr(0, pos);
        const std::string val = line.substr(pos + 1);

        if (key == "api_host")
            cfg.api_host = val;
        else if (key == "api_port_tcp")
            cfg.api_port_tcp = std::stoi(val);
        else if (key == "api_port_tls")
            cfg.api_port_tls = std::stoi(val);
        else if (key == "use_https")
            cfg.use_https = (val == "true" || val == "1");
        else if (key == "connect_timeout")
            cfg.connect_timeout = std::stoi(val);
        else if (key == "read_timeout")
            cfg.read_timeout = std::stoi(val);
        else if (key == "cache_interval_minutes")
            cfg.cache_interval_minutes = std::stoi(val);
        else if (key == "cache_directory")
            cfg.cache_directory = val;
        else if (key == "log_file")
            cfg.log_file = val;
        else if (key == "log_to_console")
            cfg.log_to_console = (val == "true" || val == "1");
        else if (key == "mp_enabled")
            cfg.mp_enabled = (val == "true" || val == "1");
    }

    return cfg;
}

}  // namespace ub
