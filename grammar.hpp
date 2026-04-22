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
};

struct number : expr {
    int value;
};

struct identifier : expr {
    std::string name;
};

struct assign_expr : expr {
    std::string name;
    std::unique_ptr<expr> value;
};

// -----------------------

struct stmt {
    virtual ~stmt() = default;
};

struct assign_stmt : stmt {
    std::string name;
    std::unique_ptr<expr> value;
};

struct if_stmt : stmt {
    std::unique_ptr<expr> condition;
    std::vector<std::unique_ptr<stmt>> then_block;
    std::vector<std::unique_ptr<stmt>> else_block;
};

struct while_stmt : stmt {
    std::unique_ptr<expr> condition;
    std::vector<std::unique_ptr<stmt>> body;
};

struct for_stmt : stmt {
    std::unique_ptr<assign_expr> init;
    std::unique_ptr<expr> condition;
    std::unique_ptr<assign_expr> update;
    std::vector<std::unique_ptr<stmt>> body;
};