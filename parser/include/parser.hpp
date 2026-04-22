#pragma once

#include <memory>
#include <vector>
#include <string>

#include "lexer.hpp"
#include "token_types.hpp"
#include "grammar.hpp"

// =======================
// Parser
// =======================

class parser {
private:
    lexer& lex;
    Token current;

    // --- core helpers ---
    void advance();
    void expect(TokenType type);
    bool is_stmt_start(TokenType type);

public:
    explicit parser(lexer& l);

    // --- top level ---
    std::vector<std::unique_ptr<stmt>> program();

    // --- statements ---
    std::vector<std::unique_ptr<stmt>> stmt_list();
    std::unique_ptr<stmt> stmt_rule();

    std::unique_ptr<stmt> assign_stmt_rule();
    std::unique_ptr<assign_expr> assign_expr_rule();

    std::unique_ptr<stmt> if_stmt_rule();
    std::unique_ptr<stmt> while_stmt_rule();
    std::unique_ptr<stmt> for_stmt_rule();

    std::vector<std::unique_ptr<stmt>> block();

    // --- expressions ---
    std::unique_ptr<expr> expr_rule();

    std::unique_ptr<expr> logical_or();
    std::unique_ptr<expr> logical_and();

    std::unique_ptr<expr> equality();
    std::unique_ptr<expr> relational();

    std::unique_ptr<expr> additive();
    std::unique_ptr<expr> term();

    std::unique_ptr<expr> factor();
};