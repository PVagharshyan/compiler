#pragma once

#include "token_types.hpp"

#include <string>
#include <unordered_map>
#include <optional>

class lexer {
    std::string src;
    size_t i = 0;

    // -------- OPERATOR AUTOMATON --------
    struct OpNode {
        std::unordered_map<char, OpNode*> next;
        std::optional<TokenType> token;
    };

    static OpNode* opRoot;

public:
    lexer(const std::string&);

    char peek() const;
    char get();
    void skipSpaces();

    Token nextToken();

private:
    Token readIdentifierOrKeyword();
    Token readNumber();
    Token readOperatorOrSymbol();

    // build automat
    static void initOperatorAutomaton();
    static void addOperator(const std::string&, TokenType);
};