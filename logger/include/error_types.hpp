#pragma once 

#include <string>

enum class ErrorType {
    LexerError,
    ParserError,
    SemanticError
};

struct Error {
    ErrorType type;
    std::string message;
    int line;
    int column;
};