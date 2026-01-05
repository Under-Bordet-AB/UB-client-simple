#include "UserInterface.hpp"

#include <iostream>
#include <limits>

namespace ub {

void UserInterface::showUsage() const {
    std::cout << "Usage: ub-client [options]" << "\n";
    std::cout << "Options:" << "\n";
    std::cout << "  --location <name>   Get location info" << "\n";
    std::cout << "  --weather <lat> <lon> Get weather info" << "\n";
    std::cout << "  --cities            Get cities" << "\n";
    std::cout << "  --surprise          Get surprise image" << "\n";
    std::cout << "\nRun without arguments for interactive mode." << std::endl;
}

void UserInterface::showWelcome() const {
    std::cout << "========================================" << std::endl;
    std::cout << "      UB-Weather Client Interface       " << std::endl;
    std::cout << "========================================" << std::endl;
}

MenuChoice UserInterface::showMenu() const {
    std::cout << "\nMain Menu:" << std::endl;
    std::cout << "1. Get Cities" << std::endl;
    std::cout << "2. Get Location Info" << std::endl;
    std::cout << "3. Get Weather Info" << std::endl;
    std::cout << "4. Get Surprise Image" << std::endl;
    std::cout << "5. Toggle Connection Type" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "\nSelect an option: ";

    int choice;
    if (!(std::cin >> choice)) {
        clearInput();
        return MenuChoice::INVALID;
    }

    switch (choice) {
        case 1:
            return MenuChoice::GET_CITIES;
        case 2:
            return MenuChoice::GET_LOCATION;
        case 3:
            return MenuChoice::GET_WEATHER;
        case 4:
            return MenuChoice::GET_SURPRISE;
        case 5:
            return MenuChoice::TOGGLE_CONNECTION;
        case 0:
            return MenuChoice::EXIT;
        default:
            return MenuChoice::INVALID;
    }
}

std::string UserInterface::askLocationName() const {
    std::cout << "Enter location name (e.g., Stockholm): ";
    std::string name;
    std::cin >> name;
    return name;
}

void UserInterface::askCoordinates(std::string& lat, std::string& lon) const {
    std::cout << "Enter latitude: ";
    std::cin >> lat;
    std::cout << "Enter longitude: ";
    std::cin >> lon;
}

void UserInterface::showResult(const std::string& mode, const std::string& result) const {
    std::cout << "\n--- Result (" << mode << ") ---" << std::endl;
    if (mode == "surprise") {
        std::cout << "[Image data received. Size: " << result.size() << " bytes]" << std::endl;
    } else {
        std::cout << result << std::endl;
    }
    std::cout << "-----------------------" << std::endl;
}

void UserInterface::showError(const std::string& message) const {
    std::cerr << "\n[ ERROR ] " << message << std::endl;
}

void UserInterface::showMessage(const std::string& message) const {
    std::cout << message << std::endl;
}

void UserInterface::clearInput() const {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

}  // namespace ub
