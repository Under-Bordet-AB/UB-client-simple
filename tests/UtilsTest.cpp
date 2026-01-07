#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "core/Utils.hpp"

using ub_client::core::Utils;

void testTrim() {
    assert(Utils::trim("  hello  ") == "hello");
    assert(Utils::trim("hello  ") == "hello");
    assert(Utils::trim("  hello") == "hello");
    assert(Utils::trim("hello") == "hello");
    assert(Utils::trim("\t\n hello \r") == "hello");
    assert(Utils::trim("") == "");
    assert(Utils::trim("   ") == "");
    std::cout << "[PASS] testTrim" << std::endl;
}

void testToInt() {
    int val = 0;
    assert(Utils::toInt("123", val) && val == 123);
    assert(Utils::toInt("-456", val) && val == -456);
    assert(Utils::toInt("0", val) && val == 0);
    assert(!Utils::toInt("abc", val));
    assert(!Utils::toInt("12abc", val));
    assert(!Utils::toInt("", val));
    assert(!Utils::toInt("2147483648", val));  // Overflow int32
    std::cout << "[PASS] testToInt" << std::endl;
}

void testUrlEncode() {
    assert(Utils::urlEncode("hello world") == "hello%20world");
    assert(Utils::urlEncode("foo/bar") == "foo%2Fbar");
    assert(Utils::urlEncode("a b") == "a%20b");
    assert(Utils::urlEncode("test@example.com") == "test%40example.com");
    std::cout << "[PASS] testUrlEncode" << std::endl;
}

int main() {
    testTrim();
    testToInt();
    testUrlEncode();
    std::cout << "=== Utils Tests Passed ===" << std::endl;
    return 0;
}
