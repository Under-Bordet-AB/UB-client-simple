# Project: C++ Secure Weather Client

**API**: [UB-WeatherServer](https://github.com/Under-Bordet-AB/UB-WeatherServer)

**Constraints**:
- Vendored mbedTLS 3.6.2 (compiled with the program)
- Makefile build system
- Simple terminal interface
- Minimal testing (ConfigManager only)

---

## 1. API Endpoints

| Endpoint | Parameters | Returns |
|----------|------------|---------|
| `/GetCities` | none | JSON: city names + coordinates |
| `/GetLocation` | `name` (req), `count`, `countryCode` | JSON: matching locations |
| `/GetWeather` | `lat` (req), `lon` (req) | JSON: weather data |
| `/GetSurprise` | none | Binary PNG image |

---

## 2. Caching Logic

**Rule**: API data refreshes at :00, :15, :30, :45 minutes.

```
current_minute = now.minute
last_boundary = (current_minute / 15) * 15   // e.g., 23 → 15, 47 → 45

if (cache_timestamp < last_boundary):
    fetch_new_data()
else:
    use_cache()
```

### JSON Cache
- Stored in `cache/` folder as JSON files per endpoint
- Filename: `get_cities.json`, `get_weather_59.33_18.07.json`, etc.

### PNG Cache (GetSurprise)
- **Deduplication by byte size**: Only one file per unique size allowed
- Filename format: `surprise_<bytesize>.png` (e.g., `surprise_45123.png`)
- Before saving: check if file with this size exists → skip if duplicate
- This prevents accumulating millions of images

---

## 3. Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                           main()                                │
│                      (Terminal Interface)                       │
└─────────────────────────────────────────────────────────────────┘
        │                    │                    │
        v                    v                    v
┌───────────────┐   ┌───────────────┐   ┌───────────────┐
│ ConfigManager │   │ ResponseCache │   │    Logger     │
└───────────────┘   └───────────────┘   └───────────────┘
                            │
                            v
              ┌─────────────────────────┐
              │       HttpClient        │
              │  (builds/parses HTTP)   │
              └───────────┬─────────────┘
                          │ uses
                          v
        ┌─────────────────────────────────────┐
        │         ISocket (interface)         │
        │  virtual: connect, send, recv, close│
        └─────────────────────────────────────┘
                 ▲                   ▲
                 │                   │
     ┌───────────┴───────┐   ┌───────┴───────────┐
     │     TcpSocket     │   │     TlsSocket     │
     │   (POSIX only)    │   │ (mbedTLS + POSIX) │
     └───────────────────┘   └───────────────────┘
        UNENCRYPTED              ENCRYPTED
```

> **Security**: `TcpSocket` and `TlsSocket` are **separate implementations**. `HttpClient` depends only on `ISocket` interface.

---

## 4. Modules

| Module | Responsibility |
|--------|----------------|
| ConfigManager | Load `config.txt`, defaults on error |
| ResponseCache | 15-min boundary check, file cache |
| Logger | Append to `app.log` |
| TcpSocket | POSIX socket wrapper |
| HttpClient | HTTP build/parse over TCP |
| TlsSocket | mbedTLS + POSIX wrapper |
| HttpsClient | HTTP build/parse over TLS |

---

## 5. File I/O

| File | Purpose |
|------|---------|
| `config.txt` | API URL, TCP/TLS ports, settings |
| `cache/*.json` | Cached API responses |
| `app.log` | Event log |

---

## 6. Testing (Minimal)

**Only test**: ConfigManager
- Valid config → loads correctly
- Invalid/missing → defaults, no crash

---

## 7. Folder Structure

```
UB-client-simple/
├── Makefile
├── config.txt
├── src/
│   ├── main.cpp
│   ├── ConfigManager.cpp/.hpp
│   ├── ResponseCache.cpp/.hpp
│   ├── Logger.cpp/.hpp
│   ├── http/
│   │   ├── TcpSocket.cpp/.hpp
│   │   └── HttpClient.cpp/.hpp
│   └── https/
│       ├── TlsSocket.cpp/.hpp
│       └── HttpsClient.cpp/.hpp
├── tests/
│   └── test_config.cpp
├── cache/
└── vendor/
    └── mbedtls/
```

---

## 8. Tech Stack

| Component | Choice | Rationale |
|-----------|--------|-----------|
| Language | C++11 | Max portability, smart pointers, RAII |
| TLS Library | mbedTLS 3.6.2 (vendored) | Lightweight, embedded-friendly |
| Build System | GNU Make | Simple, no dependencies |
| Sockets | POSIX | Standard on Linux/embedded |
| JSON | Manual parsing | No external dependencies |

---

## 9. Language Standard: C++11

**Why C++11?**
- Supported by ancient compilers (GCC 4.8+)
- Smallest binary footprint
- Still provides: `std::unique_ptr`, `auto`, lambdas, range-for

**Compiler flags**:
```makefile
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
```

---

## 10. Design Patterns

### 10.1 Strict Configuration Loading
- **Use**: ConfigManager
- **How**: If config unreadable or invalid → print error to stderr, exit program with error code
- **Why**: Prevent silent failures and ensure valid state at startup

### 10.2 Composition over Inheritance
- **Use**: HttpClient/HttpsClient use TcpSocket/TlsSocket as members
- **How**: `HttpsClient` **has-a** `TlsSocket`, not inherits from it
- **Why**: Cleaner separation, easier testing

### 10.3 Single Responsibility Principle
- **Use**: All modules
- **How**: Each class does ONE thing (socket ↔ HTTP ↔ caching ↔ logging)
- **Why**: Easier to understand, test, and modify

---

## 11. Idioms

### 11.1 RAII Wrapper Classes for C Code
- **All mbedTLS and POSIX resources** wrapped in C++ classes
- Constructor calls `*_init()`, destructor calls `*_free()`
- No raw `new`/`delete`, no manual cleanup

### 11.2 Const Correctness
- All function parameters: `const&` where possible
- Member functions that don't modify state: `const`

### 11.3 Error Handling
- Return `bool` for success/fail OR use `enum class Result`
- **No exceptions** (lighter, embedded-friendly)

### 11.4 Naming Conventions (Standard C++)

| Element | Style | Example |
|---------|-------|---------|
| Classes | PascalCase | `ConfigManager` |
| Methods | snake_case | `load_config()` |
| Variables | snake_case | `api_url` |
| Member vars | snake_case_ | `api_url_` |
| Constants | kPascalCase | `kDefaultPort` |
| Macros | UPPER_SNAKE | `MAX_BUFFER_SIZE` |
| Files | snake_case.cpp/.hpp | `tls_socket.hpp` |

### 11.5 Header Guards
- Use `#pragma once` instead of `#ifndef` include guards
- Supported by all modern compilers (and our target GCC 4.8+)
- Less boilerplate, less error-prone

---