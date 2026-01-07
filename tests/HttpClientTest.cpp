#include <cassert>
#include <iostream>
#include <memory>
#include <string>

#include "mocks/MockSocket.hpp"
#include "network/http/HttpClient.hpp"

using ub::HttpClient;
using ub::Response;
using ub::test::MockSocket;

void testSimpleGet() {
    auto mock = std::unique_ptr<MockSocket>(new MockSocket());
    MockSocket* mockPtr = mock.get();

    // Setup Mock: Valid HTTP Response
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 5\r\n"
        "\r\n"
        "Hello";

    mockPtr->connect("", 0);  // Pretend connected
    mockPtr->queueRead(response);

    HttpClient client(std::move(mock));
    Response resp = client.get("example.com", "/test");

    assert(resp.status_code == 200);
    assert(resp.body.size() == 5);
    std::string body(resp.body.begin(), resp.body.end());
    assert(body == "Hello");

    // Verify Request format
    std::string sent(mockPtr->writeBuffer.begin(), mockPtr->writeBuffer.end());
    assert(sent.find("GET /test HTTP/1.1") != std::string::npos);
    assert(sent.find("Host: example.com") != std::string::npos);

    std::cout << "[PASS] testSimpleGet" << std::endl;
}

void testBinaryImage() {
    auto mock = std::unique_ptr<MockSocket>(new MockSocket());
    MockSocket* mockPtr = mock.get();

    // Setup Mock: Partial binary PNG
    // Content-Length: 4
    // Body: \x89 P N G
    std::string headers =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 4\r\n"
        "\r\n";

    std::string body;
    body.push_back('\x89');
    body.push_back('P');
    body.push_back('N');
    body.push_back('G');

    mockPtr->connect("", 0);
    mockPtr->queueRead(headers + body);

    HttpClient client(std::move(mock));
    Response resp = client.get("img.com", "/image.png");

    assert(resp.status_code == 200);
    assert(resp.body.size() == 4);
    assert(resp.body[0] == '\x89');  // Valid char comparison

    std::cout << "[PASS] testBinaryImage" << std::endl;
}

void testChunkedHelper() {
    // TODO: Implement Chunked Transfer Encoding support in HttpClient first
    std::cout << "[SKIP] testChunkedHelper (Not Implemented)" << std::endl;
}

int main() {
    testSimpleGet();
    testBinaryImage();
    testChunkedHelper();
    std::cout << "=== HttpClient Tests Passed ===" << std::endl;
    return 0;
}
