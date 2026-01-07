CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g -Iinclude -Isrc -Isrc/core -Isrc/network -Isrc/service -Isrc/ui -Itests -isystem vendor/mbedtls/include
LDFLAGS = -Lvendor/mbedtls/library -lmbedtls -lmbedx509 -lmbedcrypto

PRETTY = ./scripts/pretty_step.sh

CORES ?= $(shell nproc)

# Explicitly exclude vendor from these lists
SRC = $(wildcard src/*.cpp) \
      $(wildcard src/core/*.cpp) \
      $(wildcard src/network/*.cpp) \
      $(wildcard src/network/http/*.cpp) \
      $(wildcard src/network/sockets/*.cpp) \
      $(wildcard src/service/*.cpp) \
      $(wildcard src/ui/*.cpp)



HEADERS = $(wildcard src/*.hpp) \
          $(wildcard src/core/*.hpp) \
          $(wildcard src/network/http/*.hpp) \
          $(wildcard src/network/sockets/*.hpp) \
          $(wildcard src/service/*.hpp) \
          $(wildcard src/ui/*.hpp) \
          $(wildcard include/*.hpp)

ALL_FILES = $(SRC) $(HEADERS)

BUILD_DIR = build
STAMP_DIR = $(BUILD_DIR)/stamps
LOG_DIR = logs
OBJ = $(SRC:%.cpp=$(BUILD_DIR)/%.o)
TARGET = ub-client

FORMAT_STAMPS = $(ALL_FILES:%=$(STAMP_DIR)/%.format.stamp)
LINT_STAMPS = $(ALL_FILES:%=$(STAMP_DIR)/%.lint.stamp)

.PHONY: all clean test libs format lint fast

# Test Configuration
TEST_SRC = $(wildcard tests/*.cpp)
TEST_OBJ = $(TEST_SRC:%.cpp=$(BUILD_DIR)/%.o)
TEST_EXES = $(TEST_SRC:tests/%.cpp=$(BUILD_DIR)/tests/%)
# Filter out main.o from project objects for testing
PROJECT_OBJ = $(filter-out $(BUILD_DIR)/src/main.o, $(OBJ))

test: $(TEST_EXES)
	@for test in $(TEST_EXES); do echo "Running $$test..."; $$test || exit 1; done

$(BUILD_DIR)/tests/%: $(BUILD_DIR)/tests/%.o $(PROJECT_OBJ)
	@mkdir -p $(dir $@)
	@$(CXX) $< $(PROJECT_OBJ) -o $@ $(LDFLAGS) > $(LOG_DIR)/link_test_$*.log 2>&1 || ($(PRETTY) FAIL "Linking Test $@" && exit 1)
	@$(PRETTY) OK "Linking Test $@"



# The 'all' target is now wrapped to handle the "Nothing to be done" case nicely
all:
	@echo "Starting building using $(CORES) cores..."
	@if ! $(MAKE) -j$(CORES) -q $(TARGET) $(FORMAT_STAMPS) $(LINT_STAMPS) 2>/dev/null; then \
		$(MAKE) -j$(CORES) --no-print-directory --silent $(TARGET) $(FORMAT_STAMPS) $(LINT_STAMPS) || ($(PRETTY) FAIL "Build failed" && exit 1); \
		$(PRETTY) OK "Build successful"; \
	else \
		$(PRETTY) OK "Everything up to date"; \
	fi


# Release Build
release:
	@echo "Building Release version..."
	@$(MAKE) --no-print-directory clean
	@$(MAKE) --no-print-directory $(TARGET) CXXFLAGS="-std=c++11 -Wall -Wextra -pedantic -O3 -DNDEBUG -Iinclude -Isrc -Isrc/core -Isrc/network -Isrc/service -Isrc/ui -Itests -isystem vendor/mbedtls/include"
	@echo "Release build complete."

fast:
	@echo "Starting fast building (no lint) using $(CORES) cores..."
	@if ! $(MAKE) -j$(CORES) -q $(TARGET) 2>/dev/null; then \
		$(MAKE) -j$(CORES) --no-print-directory --silent $(TARGET) || ($(PRETTY) FAIL "Fast build failed" && exit 1); \
		$(PRETTY) OK "Fast build successful"; \
	else \
		$(PRETTY) OK "Everything up to date"; \
	fi

$(TARGET): $(OBJ)
	@mkdir -p $(LOG_DIR)
	@$(CXX) $(OBJ) -o $@ $(LDFLAGS) > $(LOG_DIR)/link.log 2>&1 || ($(PRETTY) FAIL "Linking $@" && exit 1)
	@$(PRETTY) OK "Linking $@"

# Build vendor libs with -w (suppress all warnings) to ignore them completely
libs:
	@$(PRETTY) OK "Building vendor libraries"
	@mkdir -p $(LOG_DIR)
	@cd vendor/mbedtls && $(MAKE) -j$(CORES) --silent CFLAGS="-w -I../include -I." lib > ../../$(LOG_DIR)/mbedtls.log 2>&1 || ($(PRETTY) FAIL "Building vendor libraries" && exit 1)

$(BUILD_DIR)/%.o: %.cpp $(STAMP_DIR)/%.cpp.format.stamp
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(LOG_DIR)/$*)
	@$(CXX) $(CXXFLAGS) -c $< -o $@ > $(LOG_DIR)/$*.compile.log 2>&1 || ($(PRETTY) FAIL "Compiling $<" && exit 1)
	@$(PRETTY) OK "Compiling $<"

# Formatting
format: $(FORMAT_STAMPS)

$(STAMP_DIR)/%.format.stamp: %
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(LOG_DIR)/$*)
	@clang-format -i $< > $(LOG_DIR)/$*.format.log 2>&1 || ($(PRETTY) FAIL "Formatting $<" && exit 1)
	@$(PRETTY) OK "Formatting $<"
	@touch $@

# Linting
lint: $(TARGET) $(LINT_STAMPS)

$(STAMP_DIR)/%.lint.stamp: % $(STAMP_DIR)/%.format.stamp
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(LOG_DIR)/$*)
	@clang-tidy --quiet --use-color -header-filter='^(src|include|tests)/.*' $< -- $(CXXFLAGS) > $(LOG_DIR)/$*.lint.log 2>&1 || ($(PRETTY) FAIL "Linting $<" && exit 1)
	@$(PRETTY) OK "Linting $<"
	@touch $@

clean:
	@rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)
	@rm -f cache/*.json
	@find $(LOG_DIR) -type f \( -name "*.compile.log" -o -name "*.lint.log" -o -name "*.format.log" -o -name "link.log" -o -name "mbedtls.log" \) -delete 2>/dev/null || true
	@find . -name "*.o" -not -path "./vendor/*" -delete
	@$(PRETTY) OK "Cleaning project"
