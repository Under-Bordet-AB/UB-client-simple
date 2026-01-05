#pragma once

#include <string>

#include "ConfigManager.hpp"
#include "ResponseCache.hpp"
#include "UserInterface.hpp"

namespace ub {

class RequestManager {
public:
    void handleRequest(const std::string& mode, const std::string& endpoint, const Config& cfg,
                       ResponseCache& cache, const UserInterface& ui);
};

}  // namespace ub
