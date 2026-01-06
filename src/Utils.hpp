#pragma once

#include <iomanip>
#include <sstream>
#include <string>

namespace ub {

class Utils {
public:
    static std::string urlEncode(const std::string& value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (std::string::const_iterator i = value.begin(); i != value.end(); ++i) {
            std::string::value_type c = (*i);

            // Keep alphanumeric and other accepted characters intact
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
                continue;
            }

            // Any other characters are percent-encoded
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int((unsigned char)c);
            escaped << std::nouppercase;
        }

        return escaped.str();
    }
};

}  // namespace ub
