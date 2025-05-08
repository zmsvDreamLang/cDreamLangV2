CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Werror -Isrc
SRC_DIR = src
TARGET = main

ifeq ($(OS),Windows_NT)
    PLATFORM = windows
    ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        ARCH = x86_64
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        PLATFORM = linux
    endif
    ifeq ($(UNAME_S),Darwin)
        PLATFORM = darwin
    endif
    
    UNAME_M := $(shell uname -m)
    ifeq ($(UNAME_M),x86_64)
        ARCH = x86_64
    endif
    ifneq ($(filter %86,$(UNAME_M)),)
        ARCH = x86_64
    endif
    ifneq ($(filter arm%,$(UNAME_M)),)
        ARCH = arm64
    endif
    ifneq ($(filter aarch64%,$(UNAME_M)),)
        ARCH = arm64
    endif
endif

BUILD_DIR = build/$(PLATFORM)/$(ARCH)

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(SRC_DIR)/main.cpp $(SRC_DIR)/lexer/lexer.cpp $(SRC_DIR)/lexer/token.h $(SRC_DIR)/lexer/lexer.h $(SRC_DIR)/parser/parser.cpp $(SRC_DIR)/parser/parser.h $(SRC_DIR)/ast/ast.cpp $(SRC_DIR)/ast/ast.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_DIR)/main.cpp $(SRC_DIR)/lexer/lexer.cpp $(SRC_DIR)/parser/parser.cpp $(SRC_DIR)/ast/ast.cpp

clean:
	rm -f $(BUILD_DIR)/$(TARGET)

run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)

.PHONY: all clean run