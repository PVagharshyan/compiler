#include "parser.hpp"
#include <stdexcept>

// =======================
// constructor
// =======================

parser::parser(lexer& l) : lex(l) {
    advance();
}

// =======================
// helpers
// =======================

void parser::advance() {
    current = lex.nextToken();
}

void parser::expect(TokenType type) {
    if (current.type != type) {
        throw std::runtime_error("unexpected token: " + current.value);
    }
    advance();
}

bool parser::is_stmt_start(TokenType type) {
    return type == TokenType::Identifier ||
           type == TokenType::If ||
           type == TokenType::While ||
           type == TokenType::For;
}

// =======================
// program
// =======================

std::vector<std::unique_ptr<stmt>> parser::program() {
    auto result = stmt_list();
    expect(TokenType::EndOfFile);
    return result;
}

// =======================
// stmt_list
// =======================

std::vector<std::unique_ptr<stmt>> parser::stmt_list() {
    std::vector<std::unique_ptr<stmt>> result;

    while (is_stmt_start(current.type)) {
        result.push_back(stmt_rule());
    }

    return result;
}

// =======================
// stmt
// =======================

std::unique_ptr<stmt> parser::stmt_rule() {
    if (current.type == TokenType::Identifier)
        return assign_stmt_rule();

    if (current.type == TokenType::If)
        return if_stmt_rule();

    if (current.type == TokenType::While)
        return while_stmt_rule();

    if (current.type == TokenType::For)
        return for_stmt_rule();

    throw std::runtime_error("invalid statement");
}

// =======================
// assign_stmt
// =======================

std::unique_ptr<stmt> parser::assign_stmt_rule() {
    std::string name = current.value;
    expect(TokenType::Identifier);

    expect(TokenType::Assign);
    auto value = expr_rule();

    expect(TokenType::Semicolon);

    return std::make_unique<assign_stmt>(
        assign_stmt{name, std::move(value)}
    );
}

// =======================
// assign_expr
// =======================

std::unique_ptr<assign_expr> parser::assign_expr_rule() {
    std::string name = current.value;
    expect(TokenType::Identifier);

    expect(TokenType::Assign);
    auto value = expr_rule();

    return std::make_unique<assign_expr>(
        assign_expr{name, std::move(value)}
    );
}

// =======================
// if_stmt
// =======================

std::unique_ptr<stmt> parser::if_stmt_rule() {
    expect(TokenType::If);
    expect(TokenType::LParen);

    auto condition = expr_rule();

    expect(TokenType::RParen);

    auto then_block = block();

    std::vector<std::unique_ptr<stmt>> else_block;

    if (current.type == TokenType::Else) {
        advance();
        else_block = block();
    }

    return std::make_unique<if_stmt>(
        if_stmt{std::move(condition), std::move(then_block), std::move(else_block)}
    );
}

// =======================
// while_stmt
// =======================

std::unique_ptr<stmt> parser::while_stmt_rule() {
    expect(TokenType::While);
    expect(TokenType::LParen);

    auto condition = expr_rule();

    expect(TokenType::RParen);

    auto body = block();

    return std::make_unique<while_stmt>(
        while_stmt{std::move(condition), std::move(body)}
    );
}

// =======================
// for_stmt
// =======================

std::unique_ptr<stmt> parser::for_stmt_rule() {
    expect(TokenType::For);
    expect(TokenType::LParen);

    auto init = assign_expr_rule();
    expect(TokenType::Semicolon);

    auto condition = expr_rule();
    expect(TokenType::Semicolon);

    auto update = assign_expr_rule();

    expect(TokenType::RParen);

    auto body = block();

    return std::make_unique<for_stmt>(
        for_stmt{std::move(init), std::move(condition), std::move(update), std::move(body)}
    );
}

// =======================
// block
// =======================

std::vector<std::unique_ptr<stmt>> parser::block() {
    expect(TokenType::LBrace);
    auto result = stmt_list();
    expect(TokenType::RBrace);
    return result;
}

// =======================
// expr
// =======================

std::unique_ptr<expr> parser::expr_rule() {
    return logical_or();
}

// =======================
// logical_or
// =======================

std::unique_ptr<expr> parser::logical_or() {
    auto left = logical_and();

    while (current.type == TokenType::Operator && current.value == "||") {
        std::string op = current.value;
        advance();

        auto right = logical_and();

        left = std::make_unique<binary_expr>(
            binary_expr{op, std::move(left), std::move(right)}
        );
    }

    return left;
}

// =======================
// logical_and
// =======================

std::unique_ptr<expr> parser::logical_and() {
    auto left = equality();

    while (current.type == TokenType::Operator && current.value == "&&") {
        std::string op = current.value;
        advance();

        auto right = equality();

        left = std::make_unique<binary_expr>(
            binary_expr{op, std::move(left), std::move(right)}
        );
    }

    return left;
}

// =======================
// equality
// =======================

std::unique_ptr<expr> parser::equality() {
    auto left = relational();

    while (current.type == TokenType::Operator &&
          (current.value == "==" || current.value == "!=")) {

        std::string op = current.value;
        advance();

        auto right = relational();

        left = std::make_unique<binary_expr>(
            binary_expr{op, std::move(left), std::move(right)}
        );
    }

    return left;
}

// =======================
// relational
// =======================

std::unique_ptr<expr> parser::relational() {
    auto left = additive();

    while (current.type == TokenType::Operator &&
          (current.value == "<"  || current.value == "<=" ||
           current.value == ">"  || current.value == ">=")) {

        std::string op = current.value;
        advance();

        auto right = additive();

        left = std::make_unique<binary_expr>(
            binary_expr{op, std::move(left), std::move(right)}
        );
    }

    return left;
}

// =======================
// additive
// =======================

std::unique_ptr<expr> parser::additive() {
    auto left = term();

    while (current.type == TokenType::Operator &&
          (current.value == "+" || current.value == "-")) {

        std::string op = current.value;
        advance();

        auto right = term();

        left = std::make_unique<binary_expr>(
            binary_expr{op, std::move(left), std::move(right)}
        );
    }

    return left;
}

// =======================
// term
// =======================

std::unique_ptr<expr> parser::term() {
    auto left = factor();

    while (current.type == TokenType::Operator &&
          (current.value == "*" || current.value == "/")) {

        std::string op = current.value;
        advance();

        auto right = factor();

        left = std::make_unique<binary_expr>(
            binary_expr{op, std::move(left), std::move(right)}
        );
    }

    return left;
}

// =======================
// factor
// =======================

std::unique_ptr<expr> parser::factor() {
    if (current.type == TokenType::Number) {
        int value = std::stoi(current.value);
        advance();
        return std::make_unique<number>(number{value});
    }

    if (current.type == TokenType::Identifier) {
        std::string name = current.value;
        advance();
        return std::make_unique<identifier>(identifier{name});
    }

    if (current.type == TokenType::LParen) {
        advance();
        auto e = expr_rule();
        expect(TokenType::RParen);
        return e;
    }

    throw std::runtime_error("invalid factor: " + current.value);
}