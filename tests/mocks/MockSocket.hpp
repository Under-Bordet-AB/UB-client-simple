#pragma once

#include <cstring>
#include <deque>
#include <string>
#include <vector>

#include "network/sockets/ISocket.hpp"

namespace ub {
namespace test {

class MockSocket : public ISocket {
public:
    std::deque<char> readBuffer;
    std::vector<char> writeBuffer;
    bool connected = false;

    MockSocket() : connected(false) {}

    bool connect(const std::string&, int) override {
        connected = true;
        return true;
    }

    int write(const unsigned char* buf, size_t len) override {
        if (!connected) return -1;
        writeBuffer.insert(writeBuffer.end(), buf, buf + len);
        return static_cast<int>(len);
    }

    int read(unsigned char* buf, size_t len) override {
        if (!connected) return -1;
        if (readBuffer.empty()) return 0;  // EOF-like behavior

        size_t toRead = std::min(len, readBuffer.size());
        for (size_t i = 0; i < toRead; ++i) {
            buf[i] = readBuffer.front();
            readBuffer.pop_front();
        }
        return static_cast<int>(toRead);
    }

    void close() override {
        connected = false;
    }

    // Helper to queue data for reading
    void queueRead(const std::string& data) {
        for (char c : data) {
            readBuffer.push_back(c);
        }
    }

    // Helper to clear write buffer
    void clearWrite() {
        writeBuffer.clear();
    }
};

}  // namespace test
}  // namespace ub
