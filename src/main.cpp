#include <iostream>

#include "core/ConfigManager.hpp"
#include "core/Logger.hpp"
#include "network/NetworkFactory.hpp"
#include "service/ResponseCache.hpp"
#include "service/WeatherService.hpp"
#include "ui/AppController.hpp"
#include "ui/UserInterface.hpp"

int main(int argc, char* argv[]) {
    // 1. Load Config
    ub::ConfigResult result = ub::ConfigManager::load("client.cfg");
    if (!result.success) {
        std::cerr << "[Main] Error: " << result.error_message << std::endl;
        return 1;
    }
    ub::Config cfg = result.config;

    // 2. Initialize Dependencies
    ub::Logger::init(cfg.log_file, cfg.log_to_console);
    ub::ResponseCache cache(cfg);
    ub::NetworkFactory networkFactory(cfg);
    ub::WeatherService service(cfg, networkFactory, cache);
    ub::UserInterface ui;

    // 3. Application Controller
    ub::AppController app(ui, service, cfg);

    // 4. Run
    return app.run(argc, argv);
}
