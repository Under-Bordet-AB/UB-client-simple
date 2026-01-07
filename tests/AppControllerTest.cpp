#include <cassert>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

#include "core/ConfigManager.hpp"
#include "network/INetworkFactory.hpp"
#include "service/ResponseCache.hpp"
#include "service/WeatherService.hpp"
#include "ui/AppController.hpp"
#include "ui/IUserInterface.hpp"

using namespace ub;

#include "mocks/MockHttpClient.hpp"
#include "mocks/MockNetworkFactory.hpp"
#include "mocks/MockUserInterface.hpp"
#include "mocks/StubCache.hpp"

void testInteractiveLoop() {
    Config cfg;
    cfg.api_host = "test";
    cfg.connection_retries = 1;

    MockNetworkFactory factory;
    StubCache cache;
    WeatherService service(cfg, factory, cache);
    MockUserInterface ui;

    // Sequence: GET_CITIES -> EXIT
    ui.menuChoices.push_back(MenuChoice::GET_CITIES);
    ui.menuChoices.push_back(MenuChoice::EXIT);

    AppController app(ui, service, cfg);
    app.run(0, nullptr);  // starts interactive mode

    // Verification
    // 1. WELCOME
    // 2. RESULT:cities:OK (MockHttpClient returns OK)
    // 3. MSG:Goodbye!

    bool foundWelcome = false;
    bool foundResult = false;
    bool foundGoodbye = false;

    for (const auto& line : ui.outputs) {
        if (line == "WELCOME") foundWelcome = true;
        if (line == "RESULT:cities:OK") foundResult = true;
        if (line == "MSG:Goodbye!") foundGoodbye = true;
    }

    assert(foundWelcome);
    assert(foundResult);
    assert(foundGoodbye);

    std::cout << "[PASS] testInteractiveLoop\n";
}

int main() {
    testInteractiveLoop();
    std::cout << "=== AppController Tests Passed ===" << std::endl;
    return 0;
}
