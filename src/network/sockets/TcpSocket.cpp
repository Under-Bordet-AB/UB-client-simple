#include "TcpSocket.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iostream>

namespace ub {

TcpSocket::TcpSocket() : fd_(-1) {}

TcpSocket::~TcpSocket() {
    if (fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
}

bool TcpSocket::connect(const std::string& host, int port) {
    if (fd_ != -1) close();

    struct addrinfo hints, *res;
    int status;
    char port_str[6];
    snprintf(port_str, sizeof(port_str), "%d", port);

    std::memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(host.c_str(), port_str, &hints, &res)) != 0) {
        return false;
    }

    struct addrinfo* p;
    for (p = res; p != NULL; p = p->ai_next) {
        if ((fd_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }

        if (::connect(fd_, p->ai_addr, p->ai_addrlen) == -1) {
            ::close(fd_);
            fd_ = -1;
            continue;
        }

        break;  // Success
    }

    freeaddrinfo(res);

    if (p == NULL) {
        return false;
    }

    return true;
}

int TcpSocket::write(const unsigned char* buf, size_t len) {
    if (fd_ == -1) return -1;
    return static_cast<int>(::send(fd_, buf, len, 0));
}

int TcpSocket::read(unsigned char* buf, size_t len) {
    if (fd_ == -1) return -1;
    return static_cast<int>(::recv(fd_, buf, len, 0));
}

void TcpSocket::close() {
    if (fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
}

}  // namespace ub
