#include "utils.hpp"
#include "grammar.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

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
// AST HELPERS (PRINT)
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
// STATEMENTS (DEBUG PRINT)
// =======================

void build_stmt(const stmt* s, int indent, std::ostringstream& out) {
    if (!s) return;

    std::string ind = indent_str(indent);

    if (auto v = dynamic_cast<const var_decl_stmt*>(s)) {
        out << ind << "var_decl_stmt\n";
        out << ind << "  name: " << v->name << "\n";
        out << ind << "  value:\n";
        build_expr(v->value.get(), indent + 2, out);
    }

    else if (auto a = dynamic_cast<const assign_stmt*>(s)) {
        out << ind << "assign_stmt\n";

        out << ind << "  target:\n";
        build_expr(a->target.get(), indent + 2, out);

        out << ind << "  value:\n";
        build_expr(a->value.get(), indent + 2, out);
    }

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

    else if (auto w = dynamic_cast<const while_stmt*>(s)) {
        out << ind << "while_stmt\n";

        out << ind << "  condition:\n";
        build_expr(w->condition.get(), indent + 2, out);

        out << ind << "  body:\n";
        for (const auto& st : w->body)
            build_stmt(st.get(), indent + 2, out);
    }

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
// EXPRESSIONS (DEBUG PRINT)
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

// =====================================================
// 🚀 JSON EXPORT (FOR PYTHON VISUALIZER)
// =====================================================

static void exprToJSON(const expr* e, std::ostream& out);
static void stmtToJSON(const stmt* s, std::ostream& out);

// ---------------- EXPRESSIONS ----------------

static void exprToJSON(const expr* e, std::ostream& out) {

    if (auto n = dynamic_cast<const number*>(e)) {
        out << "{ \"type\": \"number\", \"value\": " << n->value << " }";
    }

    else if (auto id = dynamic_cast<const identifier*>(e)) {
        out << "{ \"type\": \"identifier\", \"name\": \"" << id->name << "\" }";
    }

    else if (auto b = dynamic_cast<const binary_expr*>(e)) {
        out << "{ \"type\": \"binary\", \"op\": \"" << b->op << "\", \"left\": ";
        exprToJSON(b->left.get(), out);
        out << ", \"right\": ";
        exprToJSON(b->right.get(), out);
        out << " }";
    }

    else if (auto arr = dynamic_cast<const array_literal*>(e)) {
        out << "{ \"type\": \"array\", \"elements\": [";
        for (size_t i = 0; i < arr->elements.size(); i++) {
            exprToJSON(arr->elements[i].get(), out);
            if (i + 1 < arr->elements.size()) out << ",";
        }
        out << "] }";
    }

    else if (auto acc = dynamic_cast<const array_access*>(e)) {
        out << "{ \"type\": \"array_access\", \"name\": \"" << acc->name << "\", \"index\": ";
        exprToJSON(acc->index.get(), out);
        out << " }";
    }

    else {
        out << "{ \"type\": \"unknown_expr\" }";
    }
}

// ---------------- STATEMENTS ----------------

static void stmtToJSON(const stmt* s, std::ostream& out) {

    if (auto v = dynamic_cast<const var_decl_stmt*>(s)) {
        out << "{ \"type\": \"var_decl\", \"name\": \"" << v->name << "\", \"value\": ";
        exprToJSON(v->value.get(), out);
        out << " }";
    }

    else if (auto a = dynamic_cast<const assign_stmt*>(s)) {
        out << "{ \"type\": \"assign_stmt\", \"target\": ";
        exprToJSON(a->target.get(), out);
        out << ", \"value\": ";
        exprToJSON(a->value.get(), out);
        out << " }";
    }

    else if (auto i = dynamic_cast<const if_stmt*>(s)) {
        out << "{ \"type\": \"if\", \"condition\": ";
        exprToJSON(i->condition.get(), out);

        out << ", \"then\": [";
        for (size_t k = 0; k < i->then_block.size(); k++) {
            stmtToJSON(i->then_block[k].get(), out);
            if (k + 1 < i->then_block.size()) out << ",";
        }

        out << "], \"else\": [";
        for (size_t k = 0; k < i->else_block.size(); k++) {
            stmtToJSON(i->else_block[k].get(), out);
            if (k + 1 < i->else_block.size()) out << ",";
        }

        out << "] }";
    }

    else if (auto w = dynamic_cast<const while_stmt*>(s)) {
        out << "{ \"type\": \"while\", \"condition\": ";
        exprToJSON(w->condition.get(), out);

        out << ", \"body\": [";
        for (size_t k = 0; k < w->body.size(); k++) {
            stmtToJSON(w->body[k].get(), out);
            if (k + 1 < w->body.size()) out << ",";
        }

        out << "] }";
    }

    else if (auto f = dynamic_cast<const for_stmt*>(s)) {
        out << "{ \"type\": \"for\", \"init\": ";
        exprToJSON(f->init.get(), out);

        out << ", \"condition\": ";
        exprToJSON(f->condition.get(), out);

        out << ", \"update\": ";
        exprToJSON(f->update.get(), out);

        out << ", \"body\": [";
        for (size_t k = 0; k < f->body.size(); k++) {
            stmtToJSON(f->body[k].get(), out);
            if (k + 1 < f->body.size()) out << ",";
        }

        out << "] }";
    }

    else {
        out << "{ \"type\": \"unknown_stmt\" }";
    }
}

// =====================================================
// EXPORT FUNCTIONS
// =====================================================

void exportASTJSON(const std::vector<std::unique_ptr<stmt>>& ast, const std::string& file) {
    std::ofstream out(file);

    out << "[\n";
    for (size_t i = 0; i < ast.size(); i++) {
        stmtToJSON(ast[i].get(), out);
        if (i + 1 < ast.size()) out << ",";
        out << "\n";
    }
    out << "]\n";
}

void exportTokensJSON(const std::vector<Token>& tokens, const std::string& file) {
    std::ofstream out(file);

    out << "[\n";
    for (size_t i = 0; i < tokens.size(); i++) {
        out << "  {\n";
        out << "    \"type\": \"" << tokenTypeToString(tokens[i].type) << "\",\n";
        out << "    \"value\": \"" << tokens[i].value << "\"\n";
        out << "  }";

        if (i + 1 < tokens.size()) out << ",";
        out << "\n";
    }
    out << "]\n";
}