#include "TcpSocket.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

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

    struct hostent* he;
    struct sockaddr_in server_addr;

    if ((he = gethostbyname(host.c_str())) == NULL) {
        return false;
    }

    if ((fd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return false;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr*)he->h_addr);
    std::memset(&(server_addr.sin_zero), 0, 8);

    if (::connect(fd_, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
        close();
        return false;
    }

    return true;
}

int TcpSocket::send(const unsigned char* buf, size_t len) {
    if (fd_ == -1) return -1;
    return static_cast<int>(::send(fd_, buf, len, 0));
}

int TcpSocket::recv(unsigned char* buf, size_t len) {
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
