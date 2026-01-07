#pragma once

#include <string>
#include <vector>

#include "network/http/IHttpClient.hpp"

namespace ub {

class MockHttpClient : public IHttpClient {
public:
    Response nextResponse{200, {'O', 'K'}, {}};
    std::string lastHost;
    std::string lastPath;
    int callCount = 0;

    Response get(const std::string& host, const std::string& path) override {
        lastHost = host;
        lastPath = path;
        ++callCount;
        return nextResponse;
    }
};

}  // namespace ub
