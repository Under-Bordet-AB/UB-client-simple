/**
 * @file ConfigManagerTest.cpp
 * @brief Unit tests for ConfigManager.
 *
 * Breaker Philosophy: "I am not verifying it works; I am proving it breaks."
 */

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

#include "../src/core/ConfigManager.hpp"

namespace ub {
namespace test {

// Helper to create a temporary config file
std::string createTempConfig(const std::string& content) {
    std::string path = "/tmp/ub_test_config_" + std::to_string(rand()) + ".cfg";
    std::ofstream file(path);
    file << content;
    file.close();
    return path;
}

void cleanup(const std::string& path) {
    std::remove(path.c_str());
}

// Test 1: Valid config loads correctly
void testValidConfig() {
    std::string path = createTempConfig(
        "api_host=example.com\n"
        "api_port_tcp=8080\n"
        "use_https=false\n");

    ConfigResult result = ConfigManager::load(path);

    assert(result.success == true);
    assert(result.config.api_host == "example.com");
    assert(result.config.api_port_tcp == 8080);
    assert(result.config.use_https == false);

    cleanup(path);
    std::cout << "[PASS] testValidConfig\n";
    cleanup(path);
    std::cout << "[PASS] testValidConfig\n";
}

// Test 1b: Config with whitespace and comments
void testWhitespaceConfig() {
    std::string path = createTempConfig(
        "   api_host   =   spaced.example.com   \n"
        "# This is a comment\n"
        "\n"  // Empty line
        "   api_port_tcp   =   9000   \n");

    ConfigResult result = ConfigManager::load(path);

    assert(result.success == true);
    assert(result.config.api_host == "spaced.example.com");
    assert(result.config.api_port_tcp == 9000);

    cleanup(path);
    std::cout << "[PASS] testWhitespaceConfig\n";
}

// Test 2: Missing file returns failure
void testMissingFile() {
    ConfigResult result = ConfigManager::load("/nonexistent/path/config.cfg");

    assert(result.success == false);
    assert(!result.error_message.empty());

    std::cout << "[PASS] testMissingFile\n";
}

// Test 3: Empty file returns defaults
void testEmptyFile() {
    std::string path = createTempConfig("");

    ConfigResult result = ConfigManager::load(path);

    assert(result.success == true);
    // Should have defaults
    assert(result.config.api_host == "stockholm1.onvo.se");  // Default
    assert(result.config.use_https == true);                 // Default

    cleanup(path);
    std::cout << "[PASS] testEmptyFile\n";
}

// Test 4: BREAKER TEST - Malformed integer crashes the application
// This test is designed to EXPOSE a bug: std::stoi throws on invalid input.
// Per Coding Standard Rule 5: "No Exceptions. Use return values."
// This test will FAIL (crash) until the bug is fixed.
void testMalformedInteger() {
    std::string path = createTempConfig("api_port_tcp=NOT_A_NUMBER\n");

    // If ConfigManager correctly handles errors, this should NOT crash.
    // It should return a failure or use the default value.
    ConfigResult result = ConfigManager::load(path);

    // We expect either:
    // a) success == false with an error message, OR
    // b) success == true with the default value preserved.
    // What we do NOT expect: a crash due to uncaught exception.
    (void)result;  // If we reach here without crashing, partial success.

    cleanup(path);
    std::cout << "[PASS] testMalformedInteger (survived without crash)\n";
}

// Test 5: BREAKER TEST - Extremely long value
void testExtremelyLongValue() {
    std::string longValue(100000, 'A');
    std::string path = createTempConfig("api_host=" + longValue + "\n");

    ConfigResult result = ConfigManager::load(path);

    assert(result.success == true);
    assert(result.config.api_host.length() == 100000);

    cleanup(path);
    std::cout << "[PASS] testExtremelyLongValue\n";
}

}  // namespace test
}  // namespace ub

int main() {
    std::cout << "=== ConfigManager Tests ===\n";
    ub::test::testValidConfig();
    ub::test::testWhitespaceConfig();
    ub::test::testMissingFile();
    ub::test::testEmptyFile();
    ub::test::testMalformedInteger();
    ub::test::testExtremelyLongValue();
    std::cout << "=== All Tests Passed ===\n";
    return 0;
}
