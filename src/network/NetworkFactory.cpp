#include "NetworkFactory.hpp"

#include <iostream>

#include "core/Logger.hpp"
#include "http/HttpClient.hpp"
#include "sockets/TcpSocket.hpp"
#include "sockets/TlsSocket.hpp"

namespace ub {

NetworkFactory::NetworkFactory(const Config& cfg) : cfg_(cfg) {}

std::unique_ptr<IHttpClient> NetworkFactory::createClient() {
    std::unique_ptr<ISocket> socket;

    if (cfg_.use_https) {
        std::unique_ptr<TlsSocket> tlsSocket(new TlsSocket());
        // Try common CA paths. In a real app, this should be configurable.
        tlsSocket->loadCACerts(cfg_.ca_cert_path);
        socket = std::move(tlsSocket);
    } else {
        socket = std::unique_ptr<TcpSocket>(new TcpSocket());
    }

    int port = cfg_.use_https ? cfg_.api_port_tls : cfg_.api_port_tcp;

    // Retry logic could go here if we wanted to enforce it at creation time,
    // but typically connect happens once.
    if (!socket->connect(cfg_.api_host, port)) {
        Logger::error("NetworkFactory: Failed to connect to " + cfg_.api_host + ":" +
                      std::to_string(port));
        // We return a client anyway, but the socket is closed/invalid.
        // Ideally we might return nullptr or a client in error state.
        // For now, let's return the client, and the first read/write will fail or we check
        // validity. Better: return nullptr if connection fails.
        return nullptr;
    }

    auto client = std::unique_ptr<HttpClient>(new HttpClient(std::move(socket)));
    client->setUserAgent(cfg_.user_agent);

    return client;
}

}  // namespace ub
