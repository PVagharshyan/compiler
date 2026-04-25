#include "utils.hpp"
#include "grammar.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

// =======================
// TOKEN PRINTING
// =======================

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::If: return "If";
        case TokenType::Else: return "Else";
        case TokenType::While: return "While";
        case TokenType::For: return "For";

        case TokenType::Identifier: return "Identifier";
        case TokenType::Number: return "Number";

        case TokenType::Operator: return "Operator";
        case TokenType::Assign: return "Assign";

        case TokenType::LBrace: return "LBrace";
        case TokenType::RBrace: return "RBrace";
        case TokenType::LParen: return "LParen";
        case TokenType::RParen: return "RParen";
        case TokenType::Semicolon: return "Semicolon";

        case TokenType::EndOfFile: return "EOF";
        case TokenType::Unknown: return "Unknown";

        case TokenType::Integral: return "Integral";
        case TokenType::Comma: return "Comma";
        case TokenType::LBracket: return "LBracket";
        case TokenType::RBracket: return "RBracket";
    }
    return "???";
}

std::string printTokensToString(const std::vector<Token>& tokens) {
    std::ostringstream oss;

    oss << "\nTokens:\n";
    oss << "----------------------------------------\n";
    oss << std::left
        << std::setw(5)  << "Idx"
        << std::setw(15) << "Type"
        << std::setw(15) << "Value"
        << "\n";
    oss << "----------------------------------------\n";

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& t = tokens[i];

        oss << std::left
            << std::setw(5)  << i
            << std::setw(15) << tokenTypeToString(t.type)
            << std::setw(15) << ("\"" + t.value + "\"")
            << "\n";
    }

    oss << "----------------------------------------\n";

    return oss.str();
}

// =======================
// AST HELPERS
// =======================

static std::string indent_str(int indent) {
    return std::string(indent * 2, ' ');
}

void build_stmt(const stmt* s, int indent, std::ostringstream& out);
void build_expr(const expr* e, int indent, std::ostringstream& out);

// =======================
// AST ENTRY
// =======================

std::string ast_to_string(const std::vector<std::unique_ptr<stmt>>& ast) {
    std::ostringstream out;

    out << "\n===== AST =====\n";

    for (const auto& s : ast) {
        build_stmt(s.get(), 0, out);
    }

    out << "===============\n";

    return out.str();
}

// =======================
// STATEMENTS
// =======================

void build_stmt(const stmt* s, int indent, std::ostringstream& out) {
    if (!s) return;

    std::string ind = indent_str(indent);

    // ---------------- var_decl ----------------
    if (auto v = dynamic_cast<const var_decl_stmt*>(s)) {
        out << ind << "var_decl_stmt\n";
        out << ind << "  name: " << v->name << "\n";
        out << ind << "  value:\n";
        build_expr(v->value.get(), indent + 2, out);
    }

    // ---------------- assign_stmt (IMPORTANT FIX) ----------------
    else if (auto a = dynamic_cast<const assign_stmt*>(s)) {
        out << ind << "assign_stmt\n";

        out << ind << "  target:\n";
        build_expr(a->target.get(), indent + 2, out);

        out << ind << "  value:\n";
        build_expr(a->value.get(), indent + 2, out);
    }

    // ---------------- if ----------------
    else if (auto i = dynamic_cast<const if_stmt*>(s)) {
        out << ind << "if_stmt\n";

        out << ind << "  condition:\n";
        build_expr(i->condition.get(), indent + 2, out);

        out << ind << "  then_block:\n";
        for (const auto& st : i->then_block)
            build_stmt(st.get(), indent + 2, out);

        if (!i->else_block.empty()) {
            out << ind << "  else_block:\n";
            for (const auto& st : i->else_block)
                build_stmt(st.get(), indent + 2, out);
        }
    }

    // ---------------- while ----------------
    else if (auto w = dynamic_cast<const while_stmt*>(s)) {
        out << ind << "while_stmt\n";

        out << ind << "  condition:\n";
        build_expr(w->condition.get(), indent + 2, out);

        out << ind << "  body:\n";
        for (const auto& st : w->body)
            build_stmt(st.get(), indent + 2, out);
    }

    // ---------------- for ----------------
    else if (auto f = dynamic_cast<const for_stmt*>(s)) {
        out << ind << "for_stmt\n";

        out << ind << "  init:\n";
        build_expr(f->init.get(), indent + 2, out);

        out << ind << "  condition:\n";
        build_expr(f->condition.get(), indent + 2, out);

        out << ind << "  update:\n";
        build_expr(f->update.get(), indent + 2, out);

        out << ind << "  body:\n";
        for (const auto& st : f->body)
            build_stmt(st.get(), indent + 2, out);
    }

    else {
        out << ind << "[UNKNOWN STMT]\n";
    }
}

// =======================
// EXPRESSIONS
// =======================

void build_expr(const expr* e, int indent, std::ostringstream& out) {
    if (!e) return;

    std::string ind = indent_str(indent);

    if (auto b = dynamic_cast<const binary_expr*>(e)) {
        out << ind << "binary_expr (" << b->op << ")\n";

        out << ind << "  left:\n";
        build_expr(b->left.get(), indent + 2, out);

        out << ind << "  right:\n";
        build_expr(b->right.get(), indent + 2, out);
    }

    else if (auto n = dynamic_cast<const number*>(e)) {
        out << ind << "number: " << n->value << "\n";
    }

    else if (auto id = dynamic_cast<const identifier*>(e)) {
        out << ind << "identifier: " << id->name << "\n";
    }

    else if (auto a = dynamic_cast<const assign_expr*>(e)) {
        out << ind << "assign_expr\n";
        out << ind << "  name: " << a->name << "\n";
        out << ind << "  value:\n";
        build_expr(a->value.get(), indent + 2, out);
    }

    else if (auto arr = dynamic_cast<const array_literal*>(e)) {
        out << ind << "array_literal\n";
        out << ind << "  elements:\n";
        for (const auto& el : arr->elements)
            build_expr(el.get(), indent + 2, out);
    }

    else if (auto acc = dynamic_cast<const array_access*>(e)) {
        out << ind << "array_access\n";
        out << ind << "  name: " << acc->name << "\n";
        out << ind << "  index:\n";
        build_expr(acc->index.get(), indent + 2, out);
    }

    else {
        out << ind << "[UNKNOWN EXPR]\n";
    }
}