#include <cassert>
#include <fstream>
#include <iostream>

#include "../src/ConfigManager.hpp"

void test_load_defaults() {
    // Write dummy config
    std::ofstream f("test_config.txt");
    f << "api_port_tcp=9999" << "\n";
    f.close();

    const ub::Config cfg = ub::ConfigManager::load("test_config.txt");
    assert(cfg.api_port_tcp == 9999);
    // Default check for host since we didn't set it
    // Note: Config struct default is "stockholm1.onvo.se", not localhost.
    // Let's verify what the default actually is in ConfigManager.hpp
    // It is "stockholm1.onvo.se".
    assert(cfg.api_host == "stockholm1.onvo.se");

    std::cout << "\033[0;36m[ PASS ]\033[0m test_load_defaults" << "\n";
    (void)std::remove("test_config.txt");
}

int main() {
    test_load_defaults();
    return 0;
}
