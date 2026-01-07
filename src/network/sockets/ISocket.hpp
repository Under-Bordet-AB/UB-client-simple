#pragma once

#include <cstddef>
#include <string>

namespace ub {

class ISocket {
public:
    virtual ~ISocket() = default;

    virtual bool connect(const std::string& host, int port) = 0;
    virtual int write(const unsigned char* buf, size_t len) = 0;
    virtual int read(unsigned char* buf, size_t len) = 0;
    virtual void close() = 0;
};

}  // namespace ub
