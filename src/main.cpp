#include <iostream>
#include <string>

#include "ConfigManager.hpp"
#include "Logger.hpp"
#include "RequestManager.hpp"
#include "ResponseCache.hpp"
#include "UserInterface.hpp"
#include "Utils.hpp"

int main(int argc, char* argv[]) {
    // 1. Load Config
    ub::Config cfg = ub::ConfigManager::load("config.txt");
    ub::Logger::init(cfg.log_file, cfg.log_to_console);
    ub::ResponseCache cache(cfg);
    ub::UserInterface ui;
    ub::RequestManager requestManager;

    // CLI mode
    if (argc >= 2) {
        std::string endpoint;
        const std::string mode = argv[1];

        if (mode == "--cities") {
            endpoint = "/GetCities";
        } else if (mode == "--location" && argc >= 3) {
            endpoint = "/GetLocation?name=" + ub::Utils::urlEncode(argv[2]);
        } else if (mode == "--weather" && argc >= 4) {
            endpoint = "/GetWeather?lat=" + ub::Utils::urlEncode(argv[2]) +
                       "&lon=" + ub::Utils::urlEncode(argv[3]);
        } else if (mode == "--surprise") {
            endpoint = "/GetSurprise";
        } else {
            ui.showUsage();
            return 1;
        }

        requestManager.handleRequest(mode, endpoint, cfg, cache, ui);
        return 0;
    }

    // Interactive mode
    ui.showWelcome();
    bool running = true;
    while (running) {
        ub::MenuChoice choice = ui.showMenu();
        std::string endpoint;
        std::string mode_str;

        switch (choice) {
            case ub::MenuChoice::GET_CITIES:
                mode_str = "cities";
                endpoint = "/GetCities";
                break;
            case ub::MenuChoice::GET_LOCATION: {
                std::string name = ui.askLocationName();
                mode_str = "location";
                endpoint = "/GetLocation?name=" + ub::Utils::urlEncode(name);
                break;
            }
            case ub::MenuChoice::GET_WEATHER: {
                std::string lat, lon;
                ui.askCoordinates(lat, lon);
                mode_str = "weather";
                endpoint = "/GetWeather?lat=" + ub::Utils::urlEncode(lat) +
                           "&lon=" + ub::Utils::urlEncode(lon);
                break;
            }
            case ub::MenuChoice::GET_SURPRISE:
                mode_str = "surprise";
                endpoint = "/GetSurprise";
                break;
            case ub::MenuChoice::TOGGLE_CONNECTION:
                cfg.use_https = !cfg.use_https;
                ui.showMessage(std::string("Switched to ") + (cfg.use_https ? "HTTPS" : "HTTP"));
                continue;
            case ub::MenuChoice::EXIT:
                ui.showMessage("Goodbye!");
                running = false;
                continue;
            default:
                ui.showError("Invalid choice. Try again.");
                continue;
        }

        if (!endpoint.empty()) {
            requestManager.handleRequest(mode_str, endpoint, cfg, cache, ui);
        }
    }

    return 0;
}
