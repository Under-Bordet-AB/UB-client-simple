#pragma once

#include "ArgParser.hpp"
#include "IUserInterface.hpp"
#include "core/ConfigManager.hpp"
#include "core/Logger.hpp"
#include "service/WeatherService.hpp"

namespace ub {

class AppController {
public:
    AppController(IUserInterface& ui, WeatherService& service,
                  Config& cfg);  // Take Config& to allow modifying use_https toggle

    int run(int argc, char* argv[]);

private:
    int runInteractive();
    int runSingleCommand(const Command& cmd);
    void handleCommandInLoop(MenuChoice choice);

    IUserInterface& ui_;
    WeatherService& service_;
    Config& cfg_;  // Needs to be mutable for interactive toggle
};

}  // namespace ub
