#pragma once

#include <string>

#include "TlsSocket.hpp"

namespace ub {

class HttpsClient {
public:
    HttpsClient();
    ~HttpsClient();

    std::string get(const std::string &host, int port, const std::string &path);

private:
    TlsSocket socket_;
};

}  // namespace ub
