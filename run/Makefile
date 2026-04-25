CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

TARGET := compiler

PROJECT_DIR := ..
BUILD_DIR   := build

# =========================
# PYTHON ENV (AUTO)
# =========================

VENV_DIR := .venv
PYTHON   := $(VENV_DIR)/bin/python
PIP      := $(VENV_DIR)/bin/pip

VISUALIZER := visualize.py

# =========================
# SOURCES
# =========================

LEXER_DIR        := $(PROJECT_DIR)/lexer
LOGGER_DIR       := $(PROJECT_DIR)/logger
PARSER_DIR       := $(PROJECT_DIR)/parser
INTERPRETER_DIR  := $(PROJECT_DIR)/interpreter

LEXER_SRCS    := $(wildcard $(LEXER_DIR)/src/*.cpp)
LOGGER_SRCS   := $(wildcard $(LOGGER_DIR)/src/*.cpp)
PARSER_SRCS   := $(wildcard $(PARSER_DIR)/src/*.cpp)
INTERPRETER_SRCS := $(wildcard $(INTERPRETER_DIR)/src/*.cpp)

INCDIRS := -I$(LEXER_DIR)/include \
           -I$(LOGGER_DIR)/include \
           -I$(PARSER_DIR)/include \
           -I$(INTERPRETER_DIR)/include \
           -I$(PROJECT_DIR)

# =========================
# OBJECTS
# =========================

OBJS := $(BUILD_DIR)/main.o \
        $(BUILD_DIR)/executor.o \
        $(BUILD_DIR)/utils.o \
        $(LEXER_SRCS:$(LEXER_DIR)/src/%.cpp=$(BUILD_DIR)/lexer_%.o) \
        $(LOGGER_SRCS:$(LOGGER_DIR)/src/%.cpp=$(BUILD_DIR)/logger_%.o) \
        $(PARSER_SRCS:$(PARSER_DIR)/src/%.cpp=$(BUILD_DIR)/parser_%.o) \
        $(INTERPRETER_SRCS:$(INTERPRETER_DIR)/src/%.cpp=$(BUILD_DIR)/interpreter_%.o)

# =========================
# DEFAULT BUILD
# =========================

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(PROJECT_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(BUILD_DIR)/lexer_%.o: $(LEXER_DIR)/src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(BUILD_DIR)/logger_%.o: $(LOGGER_DIR)/src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(BUILD_DIR)/parser_%.o: $(PARSER_DIR)/src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(BUILD_DIR)/interpreter_%.o: $(INTERPRETER_DIR)/src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

# =========================
# 🚀 PYTHON ENV SETUP (FIXED)
# =========================

.PHONY: setup
setup:
	@echo "Setting up Python environment..."
	python3 -m venv $(VENV_DIR)

	@echo "Upgrading pip..."
	$(VENV_DIR)/bin/python -m pip install --upgrade pip

	@echo "Installing dependencies..."
	$(VENV_DIR)/bin/pip install streamlit networkx matplotlib pyvis

	@echo "Setup complete."

# =========================
# 🚀 USER COMMANDS
# =========================

.PHONY: build
build:
	@echo "Building compiler..."
	$(MAKE) all

.PHONY: run
run: build
	@echo "Running compiler..."
	./$(TARGET)

.PHONY: view
view: setup
	@echo "Launching visualizer..."
	$(PYTHON) -m streamlit run $(VISUALIZER)

.PHONY: allinone
allinone: run view

# =========================
# CLEAN
# =========================

.PHONY: clean
clean:
	@echo "Cleaning project..."
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
	rm -f *.json
	rm -rf __pycache__
	rm -rf .streamlit
	rm -rf $(VENV_DIR)
	rm -rf ./lib
	rm -rf *.html
	@echo "Clean complete."