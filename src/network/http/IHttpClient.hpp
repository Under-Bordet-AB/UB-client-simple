#pragma once

#include <string>

#include "Response.hpp"

namespace ub {

class IHttpClient {
public:
    virtual ~IHttpClient() = default;

    virtual Response get(const std::string& host, const std::string& path) = 0;
};

}  // namespace ub
