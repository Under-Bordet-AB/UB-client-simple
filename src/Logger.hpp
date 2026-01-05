#pragma once

#include <fstream>
#include <iostream>
#include <string>

namespace ub {

class Logger {
public:
    static void init(const std::string &path, bool console);
    static void log(const std::string &message);
    static void error(const std::string &message);

private:
    static std::string log_file_;
    static bool console_;
};

}  // namespace ub
