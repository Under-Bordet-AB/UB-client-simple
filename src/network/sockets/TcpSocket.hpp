#pragma once

#include <string>

#include "ISocket.hpp"

namespace ub {

class TcpSocket : public ISocket {
public:
    TcpSocket();
    ~TcpSocket();

    bool connect(const std::string& host, int port) override;
    int write(const unsigned char* buf, size_t len) override;
    int read(unsigned char* buf, size_t len) override;
    void close() override;

private:
    int fd_;
};

}  // namespace ub
