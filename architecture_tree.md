# Architecture Tree

**Loop Counter**: Epoch 2, Round 2 / 2
**Status**: Round 2 Complete (Git Hygiene)

## Project Teleology
**Purpose**: A professional, robust, and explicitly defined C++11 Weather Client.
**Core Axioms**:
1.  **Teleology**: The goal is to fetch and process weather data correctly and securely.
2.  **Minimalism**: Reliance on standard library and vendored mbedTLS only. No unnecessary dependencies.
3.  **Robustness**: Safe handling of binary data, rigorous configuration parsing, and strict security compliance.

## Quality Policy
- **Language**: C++11 Standard (`-std=c++11`).
- **Warnings**: `-Wall -Wextra -Werror` (Zero tolerance for warnings).
- **Style**: Google C++ Style Guide (approximate).
- **Testing**: `make test` must pass.

## Coding Standards
- **Memory Management**: RAII everywhere. No raw `new`/`delete` unless wrapped immediately.
- **Strings**: `std::string` for text, `std::vector<char>` for binary.
- **Concurrency**: Single-threaded (current scope), blocking I/O with timeouts.
- **Error Handling**: Return codes or `bool` success flags. **NO EXCEPTIONS**.

## Source Tree
(Files not listed here are candidates for deletion or consolidation)

- `Makefile`: Build system
- `client.cfg`: Runtime configuration
- `src/`
  - `main.cpp`: Entry point
  - `core/`: core utilities
    - `ConfigManager.cpp/.hpp`: Configuration loader
    - `Logger.cpp/.hpp`: Logging facility
    - `Utils.hpp`: Common utilities (trim, file I/O)
  - `network/`: Network communication
    - `INetworkFactory.hpp`: Interface for network factory
    - `NetworkFactory.cpp/.hpp`: Dependency injection for network stack
    - `sockets/`: Low-level socket abstractions
      - `ISocket.hpp`: Interface
      - `TcpSocket.cpp/.hpp`: Plain TCP
      - `TlsSocket.cpp/.hpp`: TLS/SSL via mbedTLS
    - `http/`: HTTP protocol implementation
      - `IHttpClient.hpp`: Interface for HTTP clients
      - `HttpClient.cpp/.hpp`: HTTP client logic
      - `HttpBuilder.hpp`: Request construction helper
      - `Response.hpp`: HTTP Response container
  - `service/`: Business logic
    - `WeatherService.cpp/.hpp`: Domain logic for weather APIs
    - `ResponseCache.cpp/.hpp`: Caching layer
  - `ui/`: User interaction
    - `AppController.cpp/.hpp`: Main application loop
    - `ArgParser.cpp/.hpp`: Command line handling
    - `IUserInterface.hpp`: Abstract UI interface
    - `UserInterface.cpp/.hpp`: Console I/O
- `tests/`: Verification
  - `mocks/`: Test doubles
    - `MockSocket.hpp`: Socket mock for network testing
    - `MockUserInterface.hpp`: UI mock


## Top-Tier Reference
- **cURL**: For HTTP correctness.
- **Nginx**: For configuration parsing robustness.
