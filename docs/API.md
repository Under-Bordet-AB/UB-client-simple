# API Reference

Complete reference for the UB-Weather Client command-line interface and configuration.

---

## Command Line Interface

### Synopsis

```
ub-client [OPTIONS]
```

### Options

| Option | Arguments | Description |
|--------|-----------|-------------|
| *(none)* | — | Start interactive mode |
| `--cities` | — | List all available cities |
| `--location` | `<name>` | Get coordinates for a city |
| `--weather` | `<lat> <lon>` | Get weather at coordinates |
| `--surprise` | — | Download a surprise image |

### Exit Codes

| Code | Meaning |
|------|---------|
| `0` | Success |
| `1` | Error (network failure, invalid input, etc.) |

---

## Interactive Menu

When run without arguments, presents:

| Choice | Action |
|--------|--------|
| 1 | Get Cities |
| 2 | Get Location Info (prompts for city name) |
| 3 | Get Weather Info (prompts for lat/lon) |
| 4 | Get Surprise Image |
| 5 | Toggle Connection Type (HTTP/HTTPS) |
| 0 | Exit |

---

## Configuration Reference

File: `client.cfg`

### Network Settings

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `api_host` | string | — | API server hostname |
| `api_port_tcp` | int | — | Plain HTTP port |
| `api_port_tls` | int | — | HTTPS port |
| `use_https` | bool | `true` | Enable TLS |
| `ca_cert_path` | string | — | Path to CA certificate bundle |

### Connection Settings

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `connect_timeout` | int | `10` | Connection timeout (seconds) |
| `read_timeout` | int | `30` | Read timeout (seconds) |
| `connection_retries` | int | `3` | Retry attempts on failure |

### Cache Settings

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `cache_interval_minutes` | int | `15` | Cache TTL in minutes |
| `cache_directory` | string | `cache` | Directory for cached responses |

### Logging

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `log_file` | string | `app.log` | Log file path |
| `log_to_console` | bool | `true` | Also log to stdout |

### User Experience

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `user_agent` | string | `UB-WeatherClient/3.0` | HTTP User-Agent header |
| `ui_banner_text` | string | — | Custom welcome banner |
| `default_location` | string | — | Default city for prompts |
| `default_lat` | string | — | Default latitude |
| `default_lon` | string | — | Default longitude |

### Experimental

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `mp_enabled` | bool | `false` | Enable multi-processing (reserved) |

---

## API Endpoints

The client communicates with the following server endpoints:

| Endpoint | Method | Parameters | Response |
|----------|--------|------------|----------|
| `/GetCities` | GET | — | JSON list of cities |
| `/GetLocation` | GET | `name=<city>` | JSON with lat/lon |
| `/GetWeather` | GET | `lat=<lat>&lon=<lon>` | JSON weather data |
| `/GetSurprise` | GET | — | Binary PNG image |

### Response Handling

- **2xx responses**: Cached and returned
- **Non-2xx responses**: Logged as error, not cached
- **Network failure**: Triggers retry logic

---

## Examples

### Script: Check weather for multiple cities

```bash
#!/bin/bash
cities=("Stockholm" "Oslo" "Copenhagen")

for city in "${cities[@]}"; do
    echo "=== $city ==="
    coords=$(./ub-client --location "$city")
    lat=$(echo "$coords" | jq -r '.lat')
    lon=$(echo "$coords" | jq -r '.lon')
    ./ub-client --weather "$lat" "$lon"
done
```

> **Note**: Requires `jq` for JSON parsing (`apt install jq` or `brew install jq`).

### Cron: Refresh cache every 10 minutes

```cron
*/10 * * * * /path/to/ub-client --cities > /dev/null 2>&1
```

### Docker: Run with custom config

```dockerfile
FROM debian:bullseye-slim
COPY ub-client /app/
COPY client.cfg /app/
WORKDIR /app
CMD ["./ub-client"]
```
