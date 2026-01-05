#pragma once

#include <string>
#include <vector>

namespace ub {

enum class MenuChoice {
    EXIT = 0,
    GET_CITIES,
    GET_LOCATION,
    GET_WEATHER,
    GET_SURPRISE,
    TOGGLE_CONNECTION,
    INVALID
};

class UserInterface {
public:
    void showWelcome() const;
    MenuChoice showMenu() const;

    std::string askLocationName() const;
    void askCoordinates(std::string& lat, std::string& lon) const;

    void showResult(const std::string& mode, const std::string& result) const;
    void showError(const std::string& message) const;
    void showMessage(const std::string& message) const;
    void showUsage() const;

private:
    void clearInput() const;
};

}  // namespace ub
