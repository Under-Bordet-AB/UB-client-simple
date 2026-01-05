CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -Iinclude -Isrc -Isrc/http -Isrc/https -isystem vendor/mbedtls/include
LDFLAGS = -Lvendor/mbedtls/library -lmbedtls -lmbedx509 -lmbedcrypto

PRETTY = ./scripts/pretty_step.sh

CORES ?= $(shell nproc)

# Explicitly exclude vendor from these lists
SRC = $(wildcard src/*.cpp) \
      $(wildcard src/http/*.cpp) \
      $(wildcard src/https/*.cpp)

TEST_SRC = $(wildcard tests/*.cpp)

HEADERS = $(wildcard src/*.hpp) \
          $(wildcard src/http/*.hpp) \
          $(wildcard src/https/*.hpp) \
          $(wildcard include/*.hpp)

ALL_FILES = $(SRC) $(TEST_SRC) $(HEADERS)

BUILD_DIR = build
STAMP_DIR = $(BUILD_DIR)/stamps
OBJ = $(SRC:%.cpp=$(BUILD_DIR)/%.o)
TARGET = ub-client

FORMAT_STAMPS = $(ALL_FILES:%=$(STAMP_DIR)/%.format.stamp)
LINT_STAMPS = $(ALL_FILES:%=$(STAMP_DIR)/%.lint.stamp)

.PHONY: all clean test libs format lint

# The 'all' target is now wrapped to handle the "Nothing to be done" case nicely
all:
	@echo "Starting building using $(CORES) cores..."
	@if ! $(MAKE) -j$(CORES) -q $(TARGET) $(FORMAT_STAMPS) $(LINT_STAMPS) 2>/dev/null; then \
		$(MAKE) -j$(CORES) --no-print-directory --silent $(TARGET) $(FORMAT_STAMPS) $(LINT_STAMPS) || ($(PRETTY) FAIL "Build failed" && exit 1); \
		$(PRETTY) OK "Build successful"; \
	else \
		$(PRETTY) OK "Everything up to date"; \
	fi

$(TARGET): $(OBJ)
	@$(CXX) $(OBJ) -o $@ $(LDFLAGS) || ($(PRETTY) FAIL "Linking $@" && exit 1)
	@$(PRETTY) OK "Linking $@"

# Build vendor libs with -w (suppress all warnings) to ignore them completely
libs:
	@$(PRETTY) OK "Building vendor libraries"
	@cd vendor/mbedtls && $(MAKE) -j$(CORES) --silent CFLAGS="-w -I../include -I." lib > /dev/null || ($(PRETTY) FAIL "Building vendor libraries" && exit 1)

$(BUILD_DIR)/%.o: %.cpp $(STAMP_DIR)/%.cpp.format.stamp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@ || ($(PRETTY) FAIL "Compiling $<" && exit 1)
	@$(PRETTY) OK "Compiling $<"

# Formatting
format: $(FORMAT_STAMPS)

$(STAMP_DIR)/%.format.stamp: %
	@mkdir -p $(dir $@)
	@clang-format -i $< || ($(PRETTY) FAIL "Formatting $<" && exit 1)
	@$(PRETTY) OK "Formatting $<"
	@touch $@

# Linting
lint: $(TARGET) $(LINT_STAMPS)

$(STAMP_DIR)/%.lint.stamp: % $(STAMP_DIR)/%.format.stamp
	@mkdir -p $(dir $@)
	@clang-tidy --quiet --use-color -header-filter='^(src|include|tests)/.*' $< -- $(CXXFLAGS) > /dev/null 2>&1 || ($(PRETTY) FAIL "Linting $<" && exit 1)
	@$(PRETTY) OK "Linting $<"
	@touch $@

test: $(TARGET)
	@g++ -std=c++11 tests/test_config.cpp src/ConfigManager.cpp -o run_tests || ($(PRETTY) FAIL "Compiling tests" && exit 1)
	@$(PRETTY) OK "Compiling unit tests"
	@./run_tests && $(PRETTY) OK "Unit tests passed" || ($(PRETTY) FAIL "Unit tests failed" && exit 1)
	@printf "\033[0;34m[ INFO ]\033[0m Running connectivity tests...\n"
	@./tests/test_connectivity.sh || ($(PRETTY) FAIL "Connectivity tests failed" && exit 1)
	@$(PRETTY) OK "All tests passed"

clean:
	@rm -rf $(BUILD_DIR) $(TARGET) run_tests
	@rm -f cache/*.json
	@find . -name "*.o" -not -path "./vendor/*" -delete
	@$(PRETTY) OK "Cleaning project"
