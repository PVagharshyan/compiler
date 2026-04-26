# 🧠 Compiler + Interpreter + AST Visualizer Build System

This project is a modular **C++ compiler pipeline** combined with a **Python-based AST visualization system**.

It includes:
- Lexer (tokenization)
- Parser (AST construction)
- Interpreter (execution engine)
- Logger system
- Automated test runner
- Streamlit + PyVis AST visualizer

---

# ⚙️ Configuration Variables

## Compiler Settings
- `CXX := g++` → C++ compiler
- `CXXFLAGS := -std=c++17 -Wall -Wextra -O2` → compilation flags
- `TARGET := compiler` → output binary name

---

## Project Structure
- `PROJECT_DIR := ..` → root project directory
- `BUILD_DIR := build` → compiled object files
- `RUN_DIR := run` → test execution directory

---

## Required Input
- `FILE_PATH` → required input file for single execution mode

---

## Python Environment
- `VENV_DIR := .venv` → virtual environment folder
- `PYTHON := .venv/bin/python` → Python interpreter
- `PIP := .venv/bin/pip` → package manager
- `VISUALIZER := visualize.py` → Streamlit AST visualizer

---

# 📦 Source Modules

## Compiler Components
- Lexer → `lexer/src/*.cpp`
- Parser → `parser/src/*.cpp`
- Interpreter → `interpreter/src/*.cpp`
- Logger → `logger/src/*.cpp`

## Include Paths
- lexer/include
- parser/include
- interpreter/include
- logger/include
- project root

---

# 🧱 Build System

## Object Compilation Strategy
Each module is compiled into:
- `build/*.o` object files
- Separate object naming per module:
  - lexer_*.o
  - parser_*.o
  - interpreter_*.o
  - logger_*.o

---

## Main Build Target

all → builds the full compiler

compiler → links all object files into final executable

---

# 🧪 Python Environment Setup

setup → creates visualization environment

Steps:
- Create `.venv`
- Upgrade pip
- Install:
  - streamlit
  - networkx
  - matplotlib
  - pyvis

---

# 🚀 Build Commands

## Build project
build → compiles full compiler system

---

## Run all test cases
run_all → executes all files in:

run/code_examples_for_testing/

Behavior:
- Builds compiler
- Runs each test file
- Prints execution logs
- Removes:
  - ast.json
  - tokens.json

---

## Run single file

run FILE_PATH=... → strict execution mode

Behavior:
- Validates FILE_PATH
- Builds compiler
- Runs selected file with verbose mode (-v 2)
- Saves execution state
- Launches visualizer automatically

---

# 📊 AST Visualizer System

view → launches interactive AST visualizer

Behavior:
- Requires previous execution state
- Auto-installs Python dependencies if missing
- Starts Streamlit server:
  - visualize.py

Features:
- Interactive graph exploration
- Zoom and pan
- Node-level inspection
- PyVis network rendering

---

# 🧹 Clean System

clean → resets entire project state

Removes:
- build/
- compiler binary
- all .json outputs
- Python venv (.venv)
- Streamlit cache (.streamlit)
- Python cache (__pycache__)
- HTML outputs
- lib folder

---

# 🔄 Execution Workflow

Source Code (.lang)
        ↓
      Lexer
        ↓
      Parser
        ↓
   Interpreter
        ↓
 AST + Tokens (JSON)
        ↓
 Python Visualizer (Streamlit + PyVis)

---

# ⚠️ Important Rules

- FILE_PATH is mandatory for `run`
- `run_all` requires valid test directory:
  run/code_examples_for_testing/
- Visualizer only works after successful execution
- Previous run state is stored in `.last_run_state`

---

# 🛠 Example Usage

build system:
make build

run single file:
make run FILE_PATH=run/code_examples_for_testing/test.lang

run all tests:
make run_all

launch visualizer:
make view

clean project:
make clean

---

# 📌 Summary

This Makefile automates:
- Full C++ compiler build system
- Modular compilation of compiler components
- Automated test execution
- AST generation
- Python-based visualization system
- Clean project lifecycle management