# 🧠 Custom Compiler + AST Visualizer Pipeline

This project is a modular **C++ compiler/interpreter pipeline** with an integrated **Python-based AST visualizer** built using Streamlit, NetworkX, and PyVis.

It supports:
- Lexical analysis (lexer)
- Parsing (parser)
- Interpretation/execution (interpreter)
- Logging system
- AST + token generation
- Interactive visualization of AST and execution flow
- Automated test execution system

---

# 📁 Project Structure


.
├── lexer/
├── parser/
├── interpreter/
├── logger/
├── run/
│ └── code_examples_for_testing/
├── visualize.py
├── Makefile
└── build/


---

# ⚙️ Requirements

## System Requirements
- g++ (with C++17 support)
- make
- python3
- python3-venv

## Python dependencies (auto-installed via `make setup`)
- streamlit
- networkx
- matplotlib
- pyvis

---

# 🚀 Build Instructions

### 🔧 Build the compiler
```bash
make build

or simply:

make
▶️ Running the Compiler
Run a single file (STRICT MODE)
make run FILE_PATH=run/code_examples_for_testing/your_file.lang
What happens:
Builds the compiler
Runs the selected file
Generates ast.json and tokens.json
Automatically launches visualizer
Run all test cases
make run_all
What happens:
Builds compiler

Runs all files in:

run/code_examples_for_testing/
Executes each file with verbose mode
Cleans generated JSON files after execution
📊 AST Visualizer

After running a file, the visualizer is automatically launched:

make view
Features:
Interactive AST graph
Zoom / pan support
Node exploration
Streamlit-based UI
PyVis network rendering
🧪 Python Environment Setup

If needed manually:

make setup

This will:

Create .venv
Upgrade pip
Install required Python libraries
🧹 Clean Project

Remove all generated files:

make clean

This deletes:

build files
compiled binary
JSON outputs
Python cache
virtual environment
Streamlit cache
📦 Build Outputs

After compilation:

compiler → executable binary
build/ → object files
ast.json → AST output
tokens.json → lexer output
🧠 Workflow Overview
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
⚠️ Important Notes
FILE_PATH is required for make run

run_all assumes test files exist in:

run/code_examples_for_testing/
Visualizer only launches after a successful run
🛠 Example Usage
make build
make run FILE_PATH=run/code_examples_for_testing/test1.lang
make run_all
make clean
📌 Author Notes

This system is designed for:

Compiler design learning
AST visualization
Language processing experiments
Educational use in parsing/interpreting pipelines

---

If you want, I can also:
- :contentReference[oaicite:0]{index=0}
- :contentReference[oaicite:1]{index=1}
- or :contentReference[oaicite:2]{index=2}