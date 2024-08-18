CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -fopenmp

# Directories
SRC_DIR = src
BIN_DIR = bin

# Find all .cpp files in the exercises directory
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Generate executable names from source files
EXECS = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%,$(SRCS))

# Default target
all: $(EXECS)

# Rule to compile each exercise
$(BIN_DIR)/%: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean target
clean:
	rm -rf $(BIN_DIR)

# Phony targets
.PHONY: all clean

# Help target
help:
	@echo "Usage:"
	@echo "  make        - Compile all exercises"
	@echo "  make ex1    - Compile a specific exercise"
	@echo "  make clean  - Remove all compiled exercises"
	@echo "  make help   - Show this help message"

# Generate targets for individual exercises
$(SRCS:$(SRC_DIR)/%.cpp=%): %: $(BIN_DIR)/%