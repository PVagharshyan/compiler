CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

TARGET := compiler

PROJECT_DIR := ..
BUILD_DIR   := build
LEXER_DIR   := $(PROJECT_DIR)/lexer
LOGGER_DIR   := $(PROJECT_DIR)/logger

LEXER_SRCS    := $(wildcard $(LEXER_DIR)/src/*.cpp)
LEXER_INCDIRS := -I$(LEXER_DIR)/include 

LOGGER_SRCS    := $(wildcard $(LOGGER_DIR)/src/*.cpp)
LOGGER_INCDIRS := -I$(LOGGER_DIR)/include 

INCDIRS := $(LEXER_INCDIRS)
INCDIRS += $(LOGGER_INCDIRS)
INCDIRS += -I$(PROJECT_DIR)

OBJS := $(BUILD_DIR)/main.o
OBJS += $(BUILD_DIR)/executor.o
OBJS += $(LEXER_SRCS:$(LEXER_DIR)/src/%.cpp=$(BUILD_DIR)/lexer_%.o)
OBJS += $(LOGGER_SRCS:$(LOGGER_DIR)/src/%.cpp=$(BUILD_DIR)/loger_%.o)

.PHONE: all 
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(PROJECT_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(BUILD_DIR)/lexer_%.o: $(LEXER_DIR)/src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(BUILD_DIR)/loger_%.o: $(LOGGER_DIR)/src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

.PHONE: clean 
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONE: run 
run: all
	./$(TARGET)