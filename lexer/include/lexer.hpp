#pragma once

#include "token_types.hpp"

#include <string>
#include <unordered_map>

class lexer {
    std::string src;
    size_t i = 0;

public:
    lexer(const std::string&);

    char peek() const;
    char get();
    void skipSpaces();

    Token nextToken();

private:
    // ---- separated logic ----
    Token readIdentifierOrKeyword();
    Token readNumber();
    Token readOperatorOrSymbol();
};