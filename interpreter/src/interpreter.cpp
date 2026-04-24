#include "interpreter.hpp"

#include <iostream>
#include <stdexcept>

// =======================
// SCOPE MANAGEMENT
// =======================

void interpreter::push_scope() {
    scopes.emplace_back();
}

void interpreter::pop_scope() {
    if (scopes.empty()) {
        throw std::runtime_error("Scope underflow");
    }
    scopes.pop_back();
}

// =======================
// VARIABLE ACCESS
// =======================

int interpreter::get_variable(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second;
        }
    }
    throw std::runtime_error("Undefined variable: " + name);
}

void interpreter::set_variable(const std::string& name, int value) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            found->second = value;
            return;
        }
    }

    scopes.back()[name] = value;
}

// =======================
// ENTRY
// =======================

void interpreter::run(const std::vector<std::unique_ptr<stmt>>& ast) {
    scopes.clear();
    scopes.emplace_back(); // global scope

    for (const auto& s : ast) {
        exec_stmt(s.get());
    }
}

// =======================
// STATEMENTS
// =======================

void interpreter::exec_block(const std::vector<std::unique_ptr<stmt>>& block) {
    push_scope();

    for (const auto& s : block) {
        exec_stmt(s.get());
    }

    pop_scope();
}

void interpreter::exec_stmt(const stmt* s) {

    if (auto a = dynamic_cast<const assign_stmt*>(s)) {
        int value = eval_expr(a->value.get());
        set_variable(a->name, value);
    }

    else if (auto i = dynamic_cast<const if_stmt*>(s)) {
        int cond = eval_expr(i->condition.get());

        if (cond) {
            exec_block(i->then_block);
        } else {
            exec_block(i->else_block);
        }
    }

    else if (auto w = dynamic_cast<const while_stmt*>(s)) {
        while (eval_expr(w->condition.get())) {
            exec_block(w->body);
        }
    }

    else if (auto f = dynamic_cast<const for_stmt*>(s)) {

        push_scope(); // loop scope

        set_variable(f->init->name, eval_expr(f->init->value.get()));

        while (eval_expr(f->condition.get())) {
            exec_block(f->body);

            set_variable(f->update->name, eval_expr(f->update->value.get()));
        }

        pop_scope();
    }

    else {
        throw std::runtime_error("Unknown statement");
    }
}

// =======================
// EXPRESSIONS
// =======================

int interpreter::eval_expr(const expr* e) {

    if (auto n = dynamic_cast<const number*>(e)) {
        return n->value;
    }

    else if (auto id = dynamic_cast<const identifier*>(e)) {
        return get_variable(id->name);
    }

    else if (auto b = dynamic_cast<const binary_expr*>(e)) {

        int left = eval_expr(b->left.get());
        int right = eval_expr(b->right.get());

        const std::string& op = b->op;

        if (op == "+") return left + right;
        if (op == "-") return left - right;
        if (op == "*") return left * right;
        if (op == "/") return left / right;

        if (op == "==") return left == right;
        if (op == "!=") return left != right;
        if (op == "<") return left < right;
        if (op == "<=") return left <= right;
        if (op == ">") return left > right;
        if (op == ">=") return left >= right;

        if (op == "&&") return left && right;
        if (op == "||") return left || right;

        throw std::runtime_error("Unknown operator: " + op);
    }

    else if (auto a = dynamic_cast<const assign_expr*>(e)) {
        int value = eval_expr(a->value.get());
        set_variable(a->name, value);
        return value;
    }

    throw std::runtime_error("Unknown expression");
}

// =======================
// DEBUG
// =======================

void interpreter::dump_variables() const {
    std::cout << "\n=== GLOBAL VARIABLES ===\n";

    if (!scopes.empty()) {
        for (const auto& [k, v] : scopes.front()) {
            std::cout << k << " = " << v << "\n";
        }
    }

    std::cout << "========================\n";
}