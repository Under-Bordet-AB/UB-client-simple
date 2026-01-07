#pragma once

#include <memory>
#include <string>

#include "../sockets/ISocket.hpp"
#include "IHttpClient.hpp"
#include "Response.hpp"

namespace ub {

class HttpClient : public IHttpClient {
public:
    explicit HttpClient(std::unique_ptr<ISocket> socket);
    ~HttpClient();

    // Protocol agnostic GET
    Response get(const std::string& host, const std::string& path) override;

    void setUserAgent(const std::string& ua) {
        userAgent_ = ua;
    }

private:
    std::unique_ptr<ISocket> socket_;
    std::string userAgent_ = "UB-WeatherClient/1.0";
};

}  // namespace ub
