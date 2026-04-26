#include "executor.hpp"
#include "logger.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>

// =========================
// READ FILE INTO STRING
// =========================
std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    return content;
}

// =========================
// ARG PARSER (simple CLI)
// =========================
struct Config {
    int verbosity = 1;
    std::string filename;
};

Config parse_args(int argc, char** argv) {
    Config cfg;

    if (argc < 2) {
        throw std::runtime_error(
            "Usage: compiler [-v verbosity] <file>\n"
            "Example: compiler -v 2 program.txt"
        );
    }

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-v" || arg == "--verbosity") {
            if (i + 1 >= argc)
                throw std::runtime_error("Missing verbosity value");

            cfg.verbosity = std::stoi(argv[++i]);
        }
        else {
            cfg.filename = arg; // assume last non-option is file
        }
    }

    if (cfg.filename.empty()) {
        throw std::runtime_error("No input file provided");
    }

    return cfg;
}

// =========================
// MAIN
// =========================
int main(int argc, char** argv) {
    try {
        Config cfg = parse_args(argc, argv);

        LOGGER.set_verbosity(cfg.verbosity);

        std::string code = read_file(cfg.filename);

        executor exec;
        exec.run(code);

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 1;
    }
}