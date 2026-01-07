#pragma once

#include <string>
#include <vector>

#include "IUserInterface.hpp"

namespace ub {

class UserInterface : public IUserInterface {
public:
    void showWelcome(const std::string& banner) const override;
    MenuChoice showMenu() const override;

    std::string askLocationName() const override;
    void askCoordinates(std::string& lat, std::string& lon) const override;

    void showResult(const std::string& mode, const std::string& result) const override;
    void showError(const std::string& message) const override;
    void showMessage(const std::string& message) const override;
    void showUsage() const override;

private:
    void clearInput() const;
};

}  // namespace ub
