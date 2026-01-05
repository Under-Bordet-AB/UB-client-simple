#pragma once

#include <string>

#include "ISocket.hpp"

namespace ub {

class TcpSocket : public ISocket {
public:
    TcpSocket();
    ~TcpSocket();

    bool connect(const std::string &host, int port) override;
    int send(const unsigned char *buf, size_t len) override;
    int recv(unsigned char *buf, size_t len) override;
    void close() override;

private:
    int fd_;
};

}  // namespace ub
