#pragma once

#include <memory>

#include "http/IHttpClient.hpp"

namespace ub {

class INetworkFactory {
public:
    virtual ~INetworkFactory() = default;

    virtual std::unique_ptr<IHttpClient> createClient() = 0;
};

}  // namespace ub
