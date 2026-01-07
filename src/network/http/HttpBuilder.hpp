#pragma once

#include <map>
#include <sstream>
#include <string>

namespace ub {

class HttpBuilder {
public:
    HttpBuilder& method(const std::string& m) {
        method_ = m;
        return *this;
    }

    HttpBuilder& path(const std::string& p) {
        path_ = p;
        return *this;
    }

    HttpBuilder& header(const std::string& key, const std::string& value) {
        headers_[key] = value;
        return *this;
    }

    HttpBuilder& body(const std::string& b) {
        body_ = b;
        return *this;
    }

    std::string build() const {
        std::stringstream ss;
        ss << method_ << " " << path_ << " HTTP/1.1\r\n";

        for (const auto& kv : headers_) {
            ss << kv.first << ": " << kv.second << "\r\n";
        }

        if (!body_.empty()) {
            ss << "Content-Length: " << body_.length() << "\r\n";
        }

        ss << "\r\n";

        if (!body_.empty()) {
            ss << body_;
        }

        return ss.str();
    }

private:
    std::string method_ = "GET";
    std::string path_ = "/";
    std::map<std::string, std::string> headers_;
    std::string body_;
};

}  // namespace ub
