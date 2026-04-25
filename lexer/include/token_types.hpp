#pragma once 

#include <string>

enum class TokenType {
    If, Else,
    While, For,
    Integral,

    Identifier,
    Number,

    LBrace, RBrace,
    LParen, RParen,
    LBracket, RBracket,
    Comma,

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