CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude

SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN := focuser

CPP_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
HPP_FILES := $(shell find $(INC_DIR) -name '*.hpp')
ALL_SRC := $(CPP_FILES) $(HPP_FILES)

OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CPP_FILES))

all: $(BIN)

# Link
$(BIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

format:
	clang-format -style=LLVM -i $(ALL_SRC)

clean:
	rm -rf $(BUILD_DIR) $(BIN)

.PHONY: all clean format