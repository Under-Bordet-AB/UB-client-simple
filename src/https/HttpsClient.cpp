#include "HttpsClient.hpp"

#include <iostream>
#include <sstream>

#include "Logger.hpp"

namespace ub {

HttpsClient::HttpsClient() {}

HttpsClient::~HttpsClient() {}

std::string HttpsClient::get(const std::string& host, int port, const std::string& path) {
    if (!socket_.connect(host, port)) {
        Logger::error("Msg: Failed to connect to " + host);
        return "";
    }

    std::stringstream ss;
    ss << "GET " << path << " HTTP/1.1\r\n";
    ss << "Host: " << host << "\r\n";
    ss << "Connection: close\r\n";
    ss << "User-Agent: UB-WeatherClient/1.0\r\n";
    ss << "\r\n";

    const std::string request = ss.str();
    if (socket_.send((const unsigned char*)request.c_str(), request.length()) < 0) {
        Logger::error("Msg: Failed to send request");
        socket_.close();
        return "";
    }

    std::string response;
    unsigned char buf[4096];
    int len;

    while ((len = socket_.recv(buf, sizeof(buf) - 1)) > 0) {
        buf[len] = '\0';
        response.append((char*)buf);
    }
    socket_.close();

    // Simple parse - Duplicated from HttpClient (Dev mode speed!)
    size_t header_end = response.find("\r\n\r\n");
    if (header_end == std::string::npos) {
        header_end = response.find("\n\n");
    }

    if (header_end != std::string::npos) {
        size_t content_start = header_end + 4;
        if (response.find("\r\n\r\n") == std::string::npos) content_start = header_end + 2;
        return response.substr(content_start);
    }

    return response;
}

}  // namespace ub
