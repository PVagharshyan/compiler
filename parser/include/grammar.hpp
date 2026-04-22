/*
    program       -> stmt_list EOF

    stmt_list     -> stmt stmt_list | ε
    stmt          -> assign_stmt | if_stmt | while_stmt | for_stmt

    assign_stmt   -> IDENTIFIER "=" expr ";"
    assign_expr   -> IDENTIFIER "=" expr

    if_stmt       -> "if" "(" expr ")" block else_part
    else_part     -> "else" block | ε

    while_stmt    -> "while" "(" expr ")" block
    for_stmt      -> "for" "(" assign_expr ";" expr ";" assign_expr ")" block

    block         -> "{" stmt_list "}"

    expr          -> logical_or

    logical_or    -> logical_and logical_or'
    logical_or'   -> "||" logical_and logical_or' | ε

    logical_and   -> equality logical_and'
    logical_and'  -> "&&" equality logical_and' | ε

    equality      -> relational equality'
    equality'     -> ("==" | "!=") relational equality' | ε

    relational    -> additive relational'
    relational'   -> ("<" | "<=" | ">" | ">=") additive relational' | ε

    additive      -> term additive'
    additive'     -> ("+" | "-") term additive' | ε

    term          -> factor term'
    term'         -> ("*" | "/") factor term' | ε
    
    factor        -> IDENTIFIER | NUMBER | "(" expr ")"
*/

#pragma once

#include <memory>
#include <vector>
#include <string>

// =======================
// AST declarations
// =======================

struct expr {
    virtual ~expr() = default;
};

struct binary_expr : expr {
    std::string op;
    std::unique_ptr<expr> left;
    std::unique_ptr<expr> right;

    binary_expr(std::string op,
                std::unique_ptr<expr> left,
                std::unique_ptr<expr> right)
        : op(std::move(op)),
          left(std::move(left)),
          right(std::move(right)) {}
};

struct number : expr {
    int value;

    explicit number(int value)
        : value(value) {}
};

struct identifier : expr {
    std::string name;

    explicit identifier(std::string name)
        : name(std::move(name)) {}
};

struct assign_expr : expr {
    std::string name;
    std::unique_ptr<expr> value;

    assign_expr(std::string name,
                std::unique_ptr<expr> value)
        : name(std::move(name)),
          value(std::move(value)) {}
};

// -----------------------

struct stmt {
    virtual ~stmt() = default;
};

struct assign_stmt : stmt {
    std::string name;
    std::unique_ptr<expr> value;

    assign_stmt(std::string name,
                std::unique_ptr<expr> value)
        : name(std::move(name)),
          value(std::move(value)) {}
};

struct if_stmt : stmt {
    std::unique_ptr<expr> condition;
    std::vector<std::unique_ptr<stmt>> then_block;
    std::vector<std::unique_ptr<stmt>> else_block;

    if_stmt(std::unique_ptr<expr> condition,
            std::vector<std::unique_ptr<stmt>> then_block,
            std::vector<std::unique_ptr<stmt>> else_block)
        : condition(std::move(condition)),
          then_block(std::move(then_block)),
          else_block(std::move(else_block)) {}
};

struct while_stmt : stmt {
    std::unique_ptr<expr> condition;
    std::vector<std::unique_ptr<stmt>> body;

    while_stmt(std::unique_ptr<expr> condition,
               std::vector<std::unique_ptr<stmt>> body)
        : condition(std::move(condition)),
          body(std::move(body)) {}
};

struct for_stmt : stmt {
    std::unique_ptr<assign_expr> init;
    std::unique_ptr<expr> condition;
    std::unique_ptr<assign_expr> update;
    std::vector<std::unique_ptr<stmt>> body;

    for_stmt(std::unique_ptr<assign_expr> init,
             std::unique_ptr<expr> condition,
             std::unique_ptr<assign_expr> update,
             std::vector<std::unique_ptr<stmt>> body)
        : init(std::move(init)),
          condition(std::move(condition)),
          update(std::move(update)),
          body(std::move(body)) {}
};