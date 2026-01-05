# UB Weather Client (C++)

A C++ client for the apiweather server, demonstrating OOP, RAII, and custom File I/O caching.

> [!WARNING]
> **DEVELOPMENT MODE**
> This project is currently in a rapid development phase.
> - No backwards compatibility guarantees.
> - Compilation and stability are not guaranteed.
> - Tests are disabled.
> - See `.agent/development_mode.md` for agent instructions.


## Build
```bash
make
```

## Run
```bash
make run ARGS="--location Stockholm"
```

## Test
```bash
make test
```

## Structure
- `src/`: Implementation files
- `include/`: Header files
- `tests/`: Unit tests
- `docs/`: Documentation