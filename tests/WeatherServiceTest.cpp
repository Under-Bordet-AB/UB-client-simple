#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "network/INetworkFactory.hpp"
#include "network/http/IHttpClient.hpp"
#include "service/WeatherService.hpp"

using namespace ub;

#include "mocks/MockHttpClient.hpp"
#include "mocks/MockNetworkFactory.hpp"
#include "mocks/StubCache.hpp"

// =============================================================================
// Test: Basic Success (Original)
// =============================================================================
void testBasicSuccess() {
    Config cfg;
    cfg.api_host = "test.host";
    cfg.connection_retries = 3;

    MockNetworkFactory factory;
    StubCache cache;
    WeatherService service(cfg, factory, cache);

    factory.nextResponse = Response{200, {'D', 'A', 'T', 'A'}, {}};
    std::string res = service.getCities();
    assert(res == "DATA");

    std::cout << "[PASS] testBasicSuccess\n";
}

// =============================================================================
// Test: Cache Hit - verifies cached data is returned without network call
// =============================================================================
void testCacheHit() {
    Config cfg;
    cfg.api_host = "test.host";
    cfg.connection_retries = 3;

    MockNetworkFactory factory;
    StubCache cache;

    // Pre-populate cache
    cache.put("/GetCities", "CACHED_CITIES");

    WeatherService service(cfg, factory, cache);

    // Factory should NOT be called if cache hits
    factory.nextResponse = Response{200, {'N', 'E', 'T', 'W', 'O', 'R', 'K'}, {}};

    std::string res = service.getCities();

    // Should return cached data, not network data
    assert(res == "CACHED_CITIES");

    // Verify no network call was made (lastCreatedClient should be null)
    assert(factory.lastCreatedClient == nullptr);

    std::cout << "[PASS] testCacheHit\n";
}

// =============================================================================
// Test: Cache Miss - verifies network is called and response is cached
// =============================================================================
void testCacheMiss() {
    Config cfg;
    cfg.api_host = "test.host";
    cfg.connection_retries = 1;

    MockNetworkFactory factory;
    StubCache cache;
    WeatherService service(cfg, factory, cache);

    factory.nextResponse = Response{200, {'F', 'R', 'E', 'S', 'H'}, {}};

    std::string res = service.getCities();

    // Should return network data
    assert(res == "FRESH");

    // Verify data was cached
    assert(cache.get("/GetCities") == "FRESH");

    std::cout << "[PASS] testCacheMiss\n";
}

// =============================================================================
// Test: HTTP Error Not Cached - 500 response should not be cached
// =============================================================================
void testHttpErrorNotCached() {
    Config cfg;
    cfg.api_host = "test.host";
    cfg.connection_retries = 1;

    MockNetworkFactory factory;
    StubCache cache;
    WeatherService service(cfg, factory, cache);

    // Simulate server error
    factory.nextResponse = Response{500, {'E', 'R', 'R', 'O', 'R'}, {}};

    std::string res = service.getCities();

    // Should return empty on error
    assert(res.empty());

    // Verify error response was NOT cached
    assert(cache.get("/GetCities").empty());

    std::cout << "[PASS] testHttpErrorNotCached\n";
}

// =============================================================================
// Test: Retry Exhaustion - status_code=0 repeatedly exhausts retries
// =============================================================================
void testRetryExhaustion() {
    Config cfg;
    cfg.api_host = "test.host";
    cfg.connection_retries = 3;

    MockNetworkFactory factory;
    StubCache cache;
    WeatherService service(cfg, factory, cache);

    // Simulate network failure (status_code = 0 triggers retry)
    factory.nextResponse = Response{0, {}, {}};

    std::string res = service.getCities();

    // Should return empty after exhausting retries
    assert(res.empty());

    // Verify nothing was cached
    assert(cache.get("/GetCities").empty());

    // Verify exactly 3 attempts were made (matching connection_retries)
    assert(factory.createClientCallCount == 3);

    std::cout << "[PASS] testRetryExhaustion\n";
}

// =============================================================================
// Test: getSurprise returns binary data
// =============================================================================
void testSurpriseBinary() {
    Config cfg;
    cfg.api_host = "test.host";
    cfg.connection_retries = 1;

    MockNetworkFactory factory;
    StubCache cache;
    WeatherService service(cfg, factory, cache);

    // Binary data with null bytes
    std::vector<char> binaryData = {'\x89', 'P', 'N', 'G', '\x00', '\x01', '\x02'};
    factory.nextResponse = Response{200, binaryData, {}};

    std::vector<char> res = service.getSurprise();

    assert(res == binaryData);

    std::cout << "[PASS] testSurpriseBinary\n";
}

int main() {
    testBasicSuccess();
    testCacheHit();
    testCacheMiss();
    testHttpErrorNotCached();
    testRetryExhaustion();
    testSurpriseBinary();

    std::cout << "=== WeatherService Tests Passed (6/6) ===" << std::endl;
    return 0;
}
