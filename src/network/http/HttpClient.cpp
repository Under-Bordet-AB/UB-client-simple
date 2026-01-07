#include "HttpClient.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "HttpBuilder.hpp"
#include "core/Logger.hpp"
#include "core/Utils.hpp"

using ub_client::core::Utils;

namespace ub {

HttpClient::HttpClient(std::unique_ptr<ISocket> socket) : socket_(std::move(socket)) {}

HttpClient::~HttpClient() {
    if (socket_) {
        socket_->close();
    }
}

Response HttpClient::get(const std::string& host, const std::string& path) {
    std::string request = HttpBuilder()
                              .method("GET")
                              .path(path)
                              .header("Host", host)
                              .header("Connection", "close")
                              .header("User-Agent", userAgent_)
                              .build();

    if (socket_->write((const unsigned char*)request.c_str(), request.length()) < 0) {
        Logger::error("Msg: Failed to send request");
        socket_->close();
        return Response{0, {}, {}};
    }

    // Read Headers
    std::string raw_response;
    char buf[4096];
    int len;
    bool headers_received = false;
    size_t header_end_pos = 0;

    while (!headers_received && (len = socket_->read((unsigned char*)buf, sizeof(buf))) > 0) {
        raw_response.append(buf, len);
        header_end_pos = raw_response.find("\r\n\r\n");
        if (header_end_pos != std::string::npos) {
            headers_received = true;
        }
    }

    if (!headers_received) {
        socket_->close();
        return Response{0, {}, {}};
    }

    Response resp;
    resp.status_code = 0;

    // Split headers
    std::string header_block = raw_response.substr(0, header_end_pos);
    std::istringstream stream(header_block);
    std::string line;

    // Status Line
    if (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.compare(0, 9, "HTTP/1.1 ") == 0 || line.compare(0, 9, "HTTP/1.0 ") == 0) {
            try {
                size_t space = line.find(' ', 9);
                if (space != std::string::npos) {
                    resp.status_code = std::stoi(line.substr(9, space - 9));
                } else {
                    resp.status_code = std::stoi(line.substr(9));
                }
            } catch (...) {
            }
        }
    }

    // Headers
    size_t content_length = 0;
    bool has_content_length = false;

    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = Utils::trim(line.substr(0, colon));
            std::string val = Utils::trim(line.substr(colon + 1));
            resp.headers[key] = val;

            std::string key_lower = key;
            std::transform(key_lower.begin(), key_lower.end(), key_lower.begin(), ::tolower);
            if (key_lower == "content-length") {
                try {
                    content_length = std::stoul(val);
                    has_content_length = true;
                } catch (...) {
                }
            }
        }
    }

    // Body
    size_t body_start_pos = header_end_pos + 4;

    if (body_start_pos < raw_response.size()) {
        resp.body.assign(raw_response.begin() + body_start_pos, raw_response.end());
    }

    if (has_content_length) {
        size_t remaining =
            (content_length > resp.body.size()) ? (content_length - resp.body.size()) : 0;
        while (remaining > 0) {
            size_t to_read = (remaining < sizeof(buf)) ? remaining : sizeof(buf);
            len = socket_->read((unsigned char*)buf, to_read);
            if (len <= 0) break;
            resp.body.insert(resp.body.end(), buf, buf + len);
            remaining -= len;
        }
    } else {
        // Read until EOF
        while ((len = socket_->read((unsigned char*)buf, sizeof(buf))) > 0) {
            resp.body.insert(resp.body.end(), buf, buf + len);
        }
    }

    socket_->close();
    return resp;
}

}  // namespace ub
