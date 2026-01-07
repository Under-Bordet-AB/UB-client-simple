# UB-Weather Client

<div align="center">

**A professional, hardened C++11 weather client with TLS support**

[![C++11](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/license-Private-red.svg)]()
[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)]()

</div>

---

## Overview

UB-Weather Client is a robust, production-grade CLI application for querying weather data from the UB-Weather Service. Built with security and reliability as core principles, it features:

- 🔒 **TLS 1.2 encryption** via vendored mbedTLS
- ⚡ **Smart caching** to minimize network overhead
- 🎯 **Dual interface** — interactive menu or scriptable CLI
- 🛡️ **Hardened architecture** — interface-driven design, comprehensive error handling

## Quick Start

```bash
# Build
make

# Run interactive mode
./ub-client

# Run CLI mode
./ub-client --cities
./ub-client --location Stockholm
./ub-client --weather 59.32 18.06
./ub-client --surprise
```

## Features

### Dual Operation Modes

| Mode | Description | Use Case |
|------|-------------|----------|
| **Interactive** | Menu-driven interface | Human exploration |
| **CLI** | Single-command execution | Scripting, automation |

### Security

- **Certificate verification**: Strict TLS validation (`MBEDTLS_SSL_VERIFY_REQUIRED`)
- **Configurable CA path**: Point to system or custom certificates
- **Protocol toggle**: Runtime switch between HTTPS and HTTP (for testing)

### Resilience

- **Automatic retries**: Configurable retry count on connection failure
- **Smart caching**: Configurable TTL, only caches successful (200) responses
- **Graceful degradation**: Clear error messages on failure

## Installation

### Prerequisites

| Requirement | Version |
|-------------|---------|
| C++ Compiler | GCC/G++ with C++11 support |
| Build System | GNU Make |
| mbedTLS | Vendored (no external install) |

### Build

```bash
git clone <repository-url>
cd UB-client-simple
make
```

### Verify

```bash
make test   # Run all test suites
```

## Configuration

All settings are managed via `client.cfg` in the project root:

```ini
# Network
api_host=stockholm1.onvo.se
api_port_tcp=10480
api_port_tls=10443
use_https=true
ca_cert_path=/etc/ssl/certs/ca-certificates.crt

# Connection
connect_timeout=10
read_timeout=10
connection_retries=3

# Cache
cache_interval_minutes=15
cache_directory=cache

# Logging
log_file=logs/app.log
log_to_console=true

# User Experience
user_agent=UB-WeatherClient/1.0.0
ui_banner_text=UB-Weather Client Interface (v1.0.0)
default_location=Stockholm
default_lat=59.32
default_lon=18.06
```

## Usage

### Interactive Mode

```bash
./ub-client
```

Presents a menu:
```
========================================
      UB-Weather Client Interface (v1.0.0)       
========================================

Main Menu:
1. Get Cities
2. Get Location Info
3. Get Weather Info
4. Get Surprise Image
5. Toggle Connection Type
0. Exit

Select an option: 
```

### CLI Mode

```bash
# List all available cities
./ub-client --cities

# Get coordinates for a city
./ub-client --location <CityName>
./ub-client --location "New York"

# Get weather at coordinates
./ub-client --weather <lat> <lon>
./ub-client --weather 59.32 18.06

# Get a surprise image
./ub-client --surprise
```

## Architecture

```
src/
├── core/           # Utilities, config, logging
├── network/        # Socket abstraction, HTTP protocol
│   ├── sockets/    # TCP, TLS implementations
│   └── http/       # HTTP client, request builder
├── service/        # Business logic, caching
└── ui/             # CLI parsing, user interaction
```

For detailed architecture information, see [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md).

## Development

### Running Tests

```bash
make test
```

Test suites:
- `UtilsTest` — Core utility functions
- `ConfigManagerTest` — Configuration parsing
- `HttpClientTest` — HTTP protocol handling
- `WeatherServiceTest` — Business logic, caching, retries
- `AppControllerTest` — Application flow

### Code Quality

```bash
make format  # Run clang-format
make lint    # Run clang-tidy (if available)
```

### Project Standards

- **Language**: C++11 strict (`-std=c++11`)
- **Warnings**: `-Wall -Wextra -Werror`
- **Style**: Google C++ Style Guide (approximate)
- **Error Handling**: Return codes, no exceptions

## Documentation

| Document | Description |
|----------|-------------|
| [ARCHITECTURE.md](docs/ARCHITECTURE.md) | System design deep-dive |
| [API.md](docs/API.md) | CLI and configuration reference |
| [architecture_tree.md](architecture_tree.md) | Source tree specification |
| [coding_standards.md](coding_standards.md) | C++ coding conventions |

## License

Private. All rights reserved.

---

<div align="center">
<sub>Built with 🔧 and mbedTLS</sub>
</div>