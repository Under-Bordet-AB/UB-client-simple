#include "ArgParser.hpp"

namespace ub {

Command ArgParser::parse(int argc, char* argv[]) {
    if (argc < 2) {
        return {CommandType::NONE, {}};
    }

    const std::string mode = argv[1];

    if (mode == "--cities") {
        return {CommandType::GET_CITIES, {}};
    } else if (mode == "--location") {
        if (argc >= 3) {
            return {CommandType::GET_LOCATION, {argv[2]}};
        }
        return {CommandType::INVALID, {}};  // Missing Name
    } else if (mode == "--weather") {
        if (argc >= 4) {
            return {CommandType::GET_WEATHER, {argv[2], argv[3]}};
        }
        return {CommandType::INVALID, {}};  // Missing Lat/Lon
    } else if (mode == "--surprise") {
        return {CommandType::GET_SURPRISE, {}};
    }

    return {CommandType::INVALID, {}};
}

}  // namespace ub
