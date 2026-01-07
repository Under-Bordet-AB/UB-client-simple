#include "ConfigManager.hpp"

#include <algorithm>
#include <cstdlib>

#include "Utils.hpp"

using ub_client::core::Utils;

namespace ub {

ConfigResult ConfigManager::load(const std::string& path) {
    Config cfg;

    std::ifstream file(path);
    if (!file) {
        return {cfg, false, "Config file not found at " + path};
    }

    std::string line;
    while (std::getline(file, line)) {
        // Trim whitespace (simple version)
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;

        const size_t pos = line.find('=');
        if (pos == std::string::npos) continue;

        const std::string key = Utils::trim(line.substr(0, pos));
        const std::string val = Utils::trim(line.substr(pos + 1));

        if (key == "api_host")
            cfg.api_host = val;
        else if (key == "api_port_tcp")
            Utils::toInt(val, cfg.api_port_tcp);
        else if (key == "api_port_tls")
            Utils::toInt(val, cfg.api_port_tls);
        else if (key == "use_https")
            cfg.use_https = (val == "true" || val == "1");
        else if (key == "ca_cert_path")
            cfg.ca_cert_path = val;
        else if (key == "connect_timeout")
            Utils::toInt(val, cfg.connect_timeout);
        else if (key == "read_timeout")
            Utils::toInt(val, cfg.read_timeout);
        else if (key == "cache_interval_minutes")
            Utils::toInt(val, cfg.cache_interval_minutes);
        else if (key == "cache_directory")
            cfg.cache_directory = val;
        else if (key == "log_file")
            cfg.log_file = val;
        else if (key == "log_to_console")
            cfg.log_to_console = (val == "true" || val == "1");
        else if (key == "mp_enabled")
            cfg.mp_enabled = (val == "true" || val == "1");
        else if (key == "user_agent")
            cfg.user_agent = val;
        else if (key == "default_location")
            cfg.default_location = val;
        else if (key == "default_lat")
            cfg.default_lat = val;
        else if (key == "default_lon")
            cfg.default_lon = val;
        else if (key == "connection_retries")
            Utils::toInt(val, cfg.connection_retries);
        else if (key == "ui_banner_text")
            cfg.ui_banner_text = val;
    }

    return {cfg, true, ""};
}

}  // namespace ub
