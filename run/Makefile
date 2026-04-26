CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

TARGET := compiler

PROJECT_DIR := ..
BUILD_DIR   := build
RUN_DIR     := $(PROJECT_DIR)/run

# =========================
# REQUIRED INPUT (STRICT)
# =========================

FILE_PATH ?=

# =========================
# PYTHON ENV
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

LEXER_SRCS        := $(wildcard $(LEXER_DIR)/src/*.cpp)
LOGGER_SRCS       := $(wildcard $(LOGGER_DIR)/src/*.cpp)
PARSER_SRCS       := $(wildcard $(PARSER_DIR)/src/*.cpp)
INTERPRETER_SRCS  := $(wildcard $(INTERPRETER_DIR)/src/*.cpp)

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
# BUILD RULES
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
# PYTHON ENV
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
# BUILD
# =========================

.PHONY: build
build:
	@echo "Building compiler..."
	$(MAKE) all

# =========================
# RUN ALL TEST CASES
# =========================

.PHONY: run_all
run_all: build
	@echo "Running ALL test cases..."

	@if [ ! -d "$(RUN_DIR)/code_examples_for_testing" ]; then \
		echo "ERROR: Test directory not found: $(RUN_DIR)/code_examples_for_testing"; \
		exit 1; \
	fi

	@for file in $(RUN_DIR)/code_examples_for_testing/*; do \
		if [ -f $$file ]; then \
			echo "======================================"; \
			echo "Running: $$file"; \
			echo "======================================"; \
			./$(TARGET) -v 2 $$file; \
		fi \
	done

	@rm -rf ast.json tokens.json

STATE_FILE := .last_run_state

# =========================
# RUN SINGLE FILE (STRICT)
# =========================

.PHONY: run
run: build
	@if [ -z "$(FILE_PATH)" ]; then \
		echo "ERROR: FILE_PATH is not set."; \
		echo "Usage: make run FILE_PATH=run/code_examples_for_testing/file.lang"; \
		exit 1; \
	fi

	@if [ ! -f "$(FILE_PATH)" ]; then \
		echo "ERROR: File not found: $(FILE_PATH)"; \
		exit 1; \
	fi

	@echo "======================================"
	@echo "Running single file: $(FILE_PATH)"
	@echo "======================================"

	./$(TARGET) -v 2 $(FILE_PATH)

	@echo "$(FILE_PATH)" > $(STATE_FILE)

	@$(MAKE) view

# =========================
# VISUALIZER (ONLY AFTER RUN)
# =========================

.PHONY: view
view:
	@if [ ! -f "$(STATE_FILE)" ]; then \
		echo "ERROR: No execution detected."; \
		echo "You must run the compiler first using: make run FILE_PATH=..."; \
		exit 1; \
	fi

	@rm -f $(STATE_FILE)

	@echo "Setup python libs..."
	@$(MAKE) setup

	@echo "Launching visualizer..."
	$(PYTHON) -m streamlit run $(VISUALIZER)

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