#pragma once

#include "token_types.hpp"

#include <string>

class lexer {
    std::string src;
    size_t i = 0;

public:
    lexer(const std::string&);

    char peek() const;

    char get();

    void skipSpaces();

    bool isKeyword(const std::string&) const;

    Token nextToken();
};