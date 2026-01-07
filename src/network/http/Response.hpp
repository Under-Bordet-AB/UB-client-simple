#pragma once

#include <map>
#include <string>
#include <vector>

namespace ub {

struct Response {
    int status_code = 0;
    std::vector<char> body;
    std::map<std::string, std::string> headers;

    Response() = default;
    Response(int status, std::vector<char> b, std::map<std::string, std::string> h)
        : status_code(status), body(std::move(b)), headers(std::move(h)) {}

    bool isSuccess() const {
        return status_code >= 200 && status_code < 300;
    }
};

}  // namespace ub
