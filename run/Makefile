CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

TARGET := compiler

PROJECT_DIR      := ..
BUILD_DIR        := build
LEXER_DIR        := $(PROJECT_DIR)/lexer
LOGGER_DIR       := $(PROJECT_DIR)/logger
PARSER_DIR       := $(PROJECT_DIR)/parser
INTERPRETER_DIR  := $(PROJECT_DIR)/interpreter

LEXER_SRCS    := $(wildcard $(LEXER_DIR)/src/*.cpp)
LEXER_INCDIRS := -I$(LEXER_DIR)/include 

LOGGER_SRCS    := $(wildcard $(LOGGER_DIR)/src/*.cpp)
LOGGER_INCDIRS := -I$(LOGGER_DIR)/include 

PARSER_SRCS    := $(wildcard $(PARSER_DIR)/src/*.cpp)
PARSER_INCDIRS := -I$(PARSER_DIR)/include

INTERPRETER_SRCS    := $(wildcard $(INTERPRETER_DIR)/src/*.cpp)
INTERPRETER_INCDIRS := -I$(INTERPRETER_DIR)/include

INCDIRS := $(LEXER_INCDIRS)
INCDIRS += $(LOGGER_INCDIRS)
INCDIRS += -I$(PROJECT_DIR)
INCDIRS += $(PARSER_INCDIRS)
INCDIRS += $(INTERPRETER_INCDIRS)

OBJS := $(BUILD_DIR)/main.o
OBJS += $(BUILD_DIR)/executor.o
OBJS += $(BUILD_DIR)/utils.o
OBJS += $(LEXER_SRCS:$(LEXER_DIR)/src/%.cpp=$(BUILD_DIR)/lexer_%.o)
OBJS += $(LOGGER_SRCS:$(LOGGER_DIR)/src/%.cpp=$(BUILD_DIR)/loger_%.o)
OBJS += $(PARSER_SRCS:$(PARSER_DIR)/src/%.cpp=$(BUILD_DIR)/parser_%.o)
OBJS += $(INTERPRETER_SRCS:$(INTERPRETER_DIR)/src/%.cpp=$(BUILD_DIR)/interpreter_%.o)

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

$(BUILD_DIR)/parser_%.o: $(PARSER_DIR)/src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(BUILD_DIR)/interpreter_%.o: $(INTERPRETER_DIR)/src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

.PHONE: clean 
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONE: run 
run: all
	./$(TARGET)