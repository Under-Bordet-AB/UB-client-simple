#pragma once

#include <string>

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

class IUserInterface {
public:
    virtual ~IUserInterface() = default;

    virtual void showWelcome(const std::string& banner) const = 0;
    virtual MenuChoice showMenu() const = 0;

    virtual std::string askLocationName() const = 0;
    virtual void askCoordinates(std::string& lat, std::string& lon) const = 0;

    virtual void showResult(const std::string& mode, const std::string& result) const = 0;
    virtual void showError(const std::string& message) const = 0;
    virtual void showMessage(const std::string& message) const = 0;
    virtual void showUsage() const = 0;
};

}  // namespace ub
