#pragma once

#include <memory>
#include <vector>

#include "MockHttpClient.hpp"
#include "network/INetworkFactory.hpp"

namespace ub {

class MockNetworkFactory : public INetworkFactory {
public:
    MockHttpClient* lastCreatedClient = nullptr;
    Response nextResponse{200, {'O', 'K'}, {}};
    int createClientCallCount = 0;

    std::unique_ptr<IHttpClient> createClient() override {
        ++createClientCallCount;
        auto client = std::unique_ptr<MockHttpClient>(new MockHttpClient());
        client->nextResponse = nextResponse;
        lastCreatedClient = client.get();
        return std::unique_ptr<IHttpClient>(std::move(client));
    }
};

}  // namespace ub
