# C++ Coding Standards

This document defines the coding standards for the UB-client-simple project.

---

## 1. Configuration & Defaults

### The C++ Way: Config Struct with Defaults

In C, you use `#define` for defaults. In C++11, we use a **struct with in-class member initializers**:

```cpp
// config_defaults.hpp
struct Config {
    // Network
    std::string api_host = "localhost";
    int         api_port = 8080;
    bool        use_https = true;
    
    // Timeouts (seconds)
    int         connect_timeout = 10;
    int         read_timeout = 30;
    
    // Cache
    int         cache_interval_minutes = 15;
    std::string cache_directory = "./cache";
    
    // Logging
    std::string log_file = "app.log";
    bool        log_to_console = true;
};
```

### How It Works

1. **Create default config**: `Config cfg;` → all defaults applied
2. **Load from file**: `ConfigManager` reads `config.txt`, overwrites only the values found
3. **Missing values**: Keep their defaults automatically

### config.txt Format

```
# Comments start with #
api_host=weather.example.com
api_port=443
use_https=true
cache_directory=/cache
```

### ConfigManager Pattern

```cpp
class ConfigManager {
public:
    Config load(const std::string& path) {
        Config cfg;  // Start with defaults
        
        std::ifstream file(path);
        if (!file) {
            log("Config not found, using defaults");
            return cfg;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            // Skip empty lines and comments
            if (line.empty() || line[0] == '#') continue;
            
            auto pos = line.find('=');
            if (pos == std::string::npos) continue;
            
            std::string key = line.substr(0, pos);
            std::string val = line.substr(pos + 1);
            
            // Override only what's specified
            if (key == "api_host") cfg.api_host = val;
            else if (key == "api_port") cfg.api_port = std::stoi(val);
            // ... etc
        }
        return cfg;
    }
};
```

---

## 2. Header File Structure

```cpp
#pragma once

// Standard library includes (alphabetical)
#include <string>
#include <vector>

// Project includes
#include "other_module.hpp"

namespace ub {

class ClassName {
public:
    // Constructors
    ClassName();
    ~ClassName();
    
    // Public methods
    bool do_something(const std::string& input);
    
private:
    // Member variables (trailing underscore)
    std::string data_;
    int count_;
};

}  // namespace ub
```

---

## 3. Source File Structure

```cpp
#include "module_name.hpp"

// Standard library
#include <iostream>

// Project includes
#include "logger.hpp"

namespace ub {

ClassName::ClassName() : data_(""), count_(0) {
}

ClassName::~ClassName() {
}

bool ClassName::do_something(const std::string& input) {
    // Implementation
    return true;
}

}  // namespace ub
```

---

## 4. Naming Conventions

| Element | Style | Example |
|---------|-------|---------|
| Namespace | lowercase | `ub` |
| Classes | PascalCase | `ConfigManager` |
| Methods | snake_case | `load_config()` |
| Variables | snake_case | `api_url` |
| Member vars | snake_case_ | `api_url_` |
| Constants | kPascalCase | `kDefaultPort` |
| Macros | UPPER_SNAKE | `MAX_BUFFER_SIZE` |
| Files | snake_case.hpp/.cpp | `tls_socket.hpp` |

---

## 5. Error Handling

**No exceptions.** Use return values:

```cpp
// Option A: bool for simple success/fail
bool connect(const std::string& host, int port);

// Option B: enum for richer errors
enum class Result {
    kSuccess,
    kConnectionFailed,
    kTimeout,
    kInvalidInput
};

Result send_request(const std::string& data);
```

---

## 6. Resource Management (RAII)

All C resources wrapped in classes:

```cpp
class TcpSocket {
public:
    TcpSocket() : fd_(-1) {}
    
    ~TcpSocket() {
        if (fd_ >= 0) {
            close(fd_);
        }
    }
    
    // Disable copy (sockets are unique resources)
    TcpSocket(const TcpSocket&) = delete;
    TcpSocket& operator=(const TcpSocket&) = delete;
    
private:
    int fd_;
};
```

---

## 7. Include Guards

Use `#pragma once` for simplicity and compiler optimization:

```cpp
#pragma once

// ...
```

---

## 8. Comments

```cpp
// Single line comment for brief notes

/*
 * Multi-line comment for longer explanations
 * that span multiple lines.
 */

/// Doxygen-style for public API documentation
/// @param input The data to process
/// @return true on success
bool process(const std::string& input);
```
