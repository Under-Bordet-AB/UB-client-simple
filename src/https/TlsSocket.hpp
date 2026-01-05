#pragma once

#include <string>

#include "ISocket.hpp"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/error.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"

namespace ub {

class TlsSocket : public ISocket {
public:
    TlsSocket();
    ~TlsSocket();

    bool connect(const std::string &host, int port) override;
    int send(const unsigned char *buf, size_t len) override;
    int recv(unsigned char *buf, size_t len) override;
    void close() override;

private:
    mbedtls_net_context server_fd_;
    mbedtls_entropy_context entropy_;
    mbedtls_ctr_drbg_context ctr_drbg_;
    mbedtls_ssl_context ssl_;
    mbedtls_ssl_config conf_;
    bool connected_;
};

}  // namespace ub
