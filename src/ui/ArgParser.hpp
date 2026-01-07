#pragma once

#include <string>
#include <vector>

namespace ub {

enum class CommandType { NONE, GET_CITIES, GET_LOCATION, GET_WEATHER, GET_SURPRISE, INVALID };

struct Command {
    CommandType type;
    std::vector<std::string> args;

    Command(CommandType t, std::vector<std::string> a) : type(t), args(std::move(a)) {}
};

class ArgParser {
public:
    static Command parse(int argc, char* argv[]);
};

}  // namespace ub
