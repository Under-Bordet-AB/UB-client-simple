#pragma once

#include <string>

#include "TcpSocket.hpp"

namespace ub {

class HttpClient {
public:
    HttpClient();
    ~HttpClient();

    // Non-persistent connection for simplicity in first iteration
    std::string get(const std::string &host, int port, const std::string &path);

private:
    TcpSocket socket_;
};

}  // namespace ub
