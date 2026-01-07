#pragma once

#include <memory>

#include "INetworkFactory.hpp"
#include "core/ConfigManager.hpp"
#include "http/IHttpClient.hpp"

namespace ub {

class NetworkFactory : public INetworkFactory {
public:
    explicit NetworkFactory(const Config& cfg);

    // Creates a ready-to-use HttpClient with the correct socket type and connection state.
    std::unique_ptr<IHttpClient> createClient() override;

private:
    const Config& cfg_;
};

}  // namespace ub
