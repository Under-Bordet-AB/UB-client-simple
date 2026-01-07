#pragma once

#include <deque>
#include <string>
#include <vector>

#include "ui/IUserInterface.hpp"

namespace ub {

class MockUserInterface : public IUserInterface {
public:
    mutable std::deque<MenuChoice> menuChoices;
    mutable std::deque<std::string> inputs;
    mutable std::vector<std::string> outputs;

    void showWelcome(const std::string& banner) const override {
        (void)banner;
        outputs.push_back("WELCOME");
    }

    MenuChoice showMenu() const override {
        if (menuChoices.empty()) return MenuChoice::EXIT;
        MenuChoice choice = menuChoices.front();
        menuChoices.pop_front();
        return choice;
    }

    std::string askLocationName() const override {
        if (inputs.empty()) return "default_city";
        std::string s = inputs.front();
        inputs.pop_front();
        return s;
    }

    void askCoordinates(std::string& lat, std::string& lon) const override {
        lat = "0.0";
        lon = "0.0";
    }

    void showResult(const std::string& mode, const std::string& result) const override {
        outputs.push_back("RESULT:" + mode + ":" + result);
    }

    void showError(const std::string& message) const override {
        outputs.push_back("ERROR:" + message);
    }

    void showMessage(const std::string& message) const override {
        outputs.push_back("MSG:" + message);
    }

    void showUsage() const override {
        outputs.push_back("USAGE");
    }
};

}  // namespace ub
