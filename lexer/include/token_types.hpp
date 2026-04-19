#pragma once 

#include <string>

enum class TokenType {
    If, Else,
    While, For,
    Identifier,
    Number,
    LBrace, RBrace,
    LParen, RParen,
    Semicolon,
    Assign,
    Operator,
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    std::string value;
};