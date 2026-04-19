#include "error_types.hpp"
#include "error_reporter.hpp"

#include <iostream>

void error_reporter::report(ErrorType type, const std::string& msg, int line, int col) {
    errors.push_back({type, msg, line, col});
}

bool error_reporter::hasErrors() const {
    return !errors.empty();
}

void error_reporter::print() const {
    for (const auto& e : errors) {
        std::cout << "["
                    << (e.type == ErrorType::LexerError ? "Lexer" :
                        e.type == ErrorType::ParserError ? "Parser" : "Semantic")
                    << "] "
                    << "Line " << e.line << ":" << e.column
                    << " -> " << e.message << "\n";
    }
}