#include "TlsSocket.hpp"

#include <cstring>
#include <iostream>

#include "Logger.hpp"

namespace ub {

TlsSocket::TlsSocket() : connected_(false) {
    mbedtls_net_init(&server_fd_);
    mbedtls_ssl_init(&ssl_);
    mbedtls_ssl_config_init(&conf_);
    mbedtls_ctr_drbg_init(&ctr_drbg_);
    mbedtls_entropy_init(&entropy_);

    const char* pers = "ub_client";
    mbedtls_ctr_drbg_seed(&ctr_drbg_, mbedtls_entropy_func, &entropy_, (const unsigned char*)pers,
                          strlen(pers));
}

TlsSocket::~TlsSocket() {
    if (connected_) {
        mbedtls_ssl_close_notify(&ssl_);
        connected_ = false;
    }
    mbedtls_net_free(&server_fd_);
    mbedtls_ssl_free(&ssl_);
    mbedtls_ssl_config_free(&conf_);
    mbedtls_ctr_drbg_free(&ctr_drbg_);
    mbedtls_entropy_free(&entropy_);
}

bool TlsSocket::connect(const std::string& host, int port) {
    int ret;
    const std::string port_str = std::to_string(port);

    if (mbedtls_net_connect(&server_fd_, host.c_str(), port_str.c_str(), MBEDTLS_NET_PROTO_TCP) !=
        0) {
        Logger::error("mbedtls_net_connect failed");
        return false;
    }

    if (mbedtls_ssl_config_defaults(&conf_, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM,
                                    MBEDTLS_SSL_PRESET_DEFAULT) != 0) {
        Logger::error("mbedtls_ssl_config_defaults failed");
        return false;
    }

    mbedtls_ssl_conf_authmode(&conf_,
                              MBEDTLS_SSL_VERIFY_OPTIONAL);  // For dev mode, maybe lax? Design says
                                                             // "Vendored mbedTLS", implies we might
                                                             // have CA certs? But for now optional.
    mbedtls_ssl_conf_rng(&conf_, mbedtls_ctr_drbg_random, &ctr_drbg_);

    if (mbedtls_ssl_setup(&ssl_, &conf_) != 0) {
        Logger::error("mbedtls_ssl_setup failed");
        return false;
    }

    if (mbedtls_ssl_set_hostname(&ssl_, host.c_str()) != 0) {
        Logger::error("mbedtls_ssl_set_hostname failed");
        return false;
    }

    mbedtls_ssl_set_bio(&ssl_, &server_fd_, mbedtls_net_send, mbedtls_net_recv, NULL);

    while ((ret = mbedtls_ssl_handshake(&ssl_)) != 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
            Logger::error("mbedtls_ssl_handshake failed");
            return false;
        }
    }

    connected_ = true;
    return true;
}

int TlsSocket::send(const unsigned char* buf, size_t len) {
    if (!connected_) return -1;

    int ret;
    while ((ret = mbedtls_ssl_write(&ssl_, buf, len)) <= 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
            return -1;
        }
    }
    return ret;
}

int TlsSocket::recv(unsigned char* buf, size_t len) {
    if (!connected_) return -1;

    int ret;
    // mbedtls_ssl_read returns 0 on close-notify, or error
    // We want to behave like POSIX recv
    ret = mbedtls_ssl_read(&ssl_, buf, len);

    if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE) {
        return -1;  // Should handle non-blocking, but for now simple blocking logic
    }

    if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
        return 0;
    }

    if (ret < 0) {
        return -1;
    }

    return ret;
}

void TlsSocket::close() {
    if (connected_) {
        mbedtls_ssl_close_notify(&ssl_);
        connected_ = false;
    }
    // mbedtls_net_free closes the fd, but we call it in dtor.
    // Usually good to have explicit close.
}

}  // namespace ub
