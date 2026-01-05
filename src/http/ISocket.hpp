#pragma once

#include <cstddef>
#include <string>

namespace ub {

class ISocket {
public:
    virtual ~ISocket() = default;

    virtual bool connect(const std::string& host, int port) = 0;
    virtual int send(const unsigned char* buf, size_t len) = 0;
    virtual int recv(unsigned char* buf, size_t len) = 0;
    virtual void close() = 0;
};

}  // namespace ub
