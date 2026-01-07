#include "AppController.hpp"

#include <iostream>

namespace ub {

AppController::AppController(IUserInterface& ui, WeatherService& service, Config& cfg)
    : ui_(ui), service_(service), cfg_(cfg) {}

int AppController::run(int argc, char* argv[]) {
    Command cmd = ArgParser::parse(argc, argv);

    if (cmd.type == CommandType::NONE) {
        return runInteractive();
    }

    if (cmd.type == CommandType::INVALID) {
        ui_.showUsage();
        return 1;
    }

    return runSingleCommand(cmd);
}

int AppController::runInteractive() {
    ui_.showWelcome(cfg_.ui_banner_text);
    bool running = true;

    while (running) {
        MenuChoice choice = ui_.showMenu();
        if (choice == MenuChoice::EXIT) {
            ui_.showMessage("Goodbye!");
            running = false;
        } else if (choice == MenuChoice::TOGGLE_CONNECTION) {
            cfg_.use_https = !cfg_.use_https;
            ui_.showMessage(std::string("Switched to ") + (cfg_.use_https ? "HTTPS" : "HTTP"));
        } else if (choice == MenuChoice::INVALID) {
            ui_.showError("Invalid choice. Try again.");
        } else {
            handleCommandInLoop(choice);
        }
    }
    return 0;
}

void AppController::handleCommandInLoop(MenuChoice choice) {
    std::string result;
    std::string mode_label;

    switch (choice) {
        case MenuChoice::GET_CITIES:
            mode_label = "cities";
            result = service_.getCities();
            break;
        case MenuChoice::GET_LOCATION: {
            std::string name = ui_.askLocationName();
            mode_label = "location";
            result = service_.getLocation(name);
            break;
        }
        case MenuChoice::GET_WEATHER: {
            std::string lat, lon;
            ui_.askCoordinates(lat, lon);
            mode_label = "weather";
            result = service_.getWeather(lat, lon);
            break;
        }
        case MenuChoice::GET_SURPRISE:
            mode_label = "surprise";
            {
                auto v = service_.getSurprise();
                result = std::string(v.begin(), v.end());
            }
            break;
        default:
            return;
    }

    if (!result.empty()) {
        ui_.showResult(mode_label, result);
    } else {
        ui_.showError("Failed to fetch data.");
    }
}

int AppController::runSingleCommand(const Command& cmd) {
    std::string result;
    std::string mode_label;

    switch (cmd.type) {
        case CommandType::GET_CITIES:
            mode_label = "--cities";
            result = service_.getCities();
            break;
        case CommandType::GET_LOCATION:
            mode_label = "--location";
            result = service_.getLocation(cmd.args[0]);
            break;
        case CommandType::GET_WEATHER:
            mode_label = "--weather";
            result = service_.getWeather(cmd.args[0], cmd.args[1]);
            break;
        case CommandType::GET_SURPRISE:
            mode_label = "--surprise";
            {
                auto v = service_.getSurprise();
                result = std::string(v.begin(), v.end());
            }
            break;
        default:
            return 1;
    }

    if (!result.empty()) {
        ui_.showResult(mode_label == "--surprise" ? "surprise" : mode_label, result);
        return 0;
    } else {
        ui_.showError("Failed to fetch data.");
        return 1;
    }
}

}  // namespace ub
