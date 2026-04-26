#include "interpreter.hpp"

#include <iostream>
#include <stdexcept>
#include <variant>

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

value interpreter::get_variable(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second;
        }
    }
    throw std::runtime_error("Undefined variable: " + name);
}

void interpreter::set_variable(const std::string& name, const value& val) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            found->second = val;
            return;
        }
    }
    throw std::runtime_error("Assignment to undefined variable: " + name);
}

// =======================
// ARRAY UPDATE
// =======================

void interpreter::set_array_element(const std::string& name, int index, int new_value) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(name);

        if (found != it->end()) {
            if (!std::holds_alternative<std::vector<int>>(found->second)) {
                throw std::runtime_error("Not an array: " + name);
            }

            auto& arr = std::get<std::vector<int>>(found->second);

            if (index < 0 || index >= (int)arr.size()) {
                throw std::runtime_error("Array index out of bounds");
            }

            arr[index] = new_value;
            return;
        }
    }

    throw std::runtime_error("Undefined array: " + name);
}

// =======================
// ENTRY
// =======================

void interpreter::run(const std::vector<std::unique_ptr<stmt>>& ast) {
    scopes.clear();
    scopes.emplace_back();

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

    if (auto v = dynamic_cast<const var_decl_stmt*>(s)) {
        auto& cur = scopes.back();

        if (cur.count(v->name)) {
            throw std::runtime_error("Variable already declared: " + v->name);
        }

        cur[v->name] = eval_expr(v->value.get());
    }

    else if (auto a = dynamic_cast<const assign_stmt*>(s)) {

        if (auto acc = dynamic_cast<const array_access*>(a->target.get())) {
            int idx = as_int(eval_expr(acc->index.get()));
            int val = as_int(eval_expr(a->value.get()));
            set_array_element(acc->name, idx, val);
            return;
        }

        if (auto id = dynamic_cast<const identifier*>(a->target.get())) {
            set_variable(id->name, eval_expr(a->value.get()));
            return;
        }

        throw std::runtime_error("Invalid assignment target");
    }

    else if (auto i = dynamic_cast<const if_stmt*>(s)) {
        if (as_int(eval_expr(i->condition.get()))) {
            exec_block(i->then_block);
        } else {
            exec_block(i->else_block);
        }
    }

    else if (auto w = dynamic_cast<const while_stmt*>(s)) {
        while (as_int(eval_expr(w->condition.get()))) {
            exec_block(w->body);
        }
    }

    else if (auto f = dynamic_cast<const for_stmt*>(s)) {
        push_scope();

        set_variable(f->init->name, eval_expr(f->init->value.get()));

        while (as_int(eval_expr(f->condition.get()))) {
            exec_block(f->body);
            set_variable(f->update->name, eval_expr(f->update->value.get()));
        }

        pop_scope();
    }

    else {
        throw std::runtime_error("Unknown statement type");
    }
}

// =======================
// EXPRESSIONS
// =======================

int interpreter::as_int(const value& v) {
    if (!std::holds_alternative<int>(v)) {
        throw std::runtime_error("Expected integer");
    }
    return std::get<int>(v);
}

value interpreter::eval_expr(const expr* e) {

    if (auto n = dynamic_cast<const number*>(e)) {
        return n->value;
    }

    else if (auto id = dynamic_cast<const identifier*>(e)) {
        return get_variable(id->name);
    }

    else if (auto b = dynamic_cast<const binary_expr*>(e)) {

        if (b->op == "&&") {
            int l = as_int(eval_expr(b->left.get()));
            if (!l) return 0; // short-circuit
            return as_int(eval_expr(b->right.get())) ? 1 : 0;
        }

        if (b->op == "||") {
            int l = as_int(eval_expr(b->left.get()));
            if (l) return 1; // short-circuit
            return as_int(eval_expr(b->right.get())) ? 1 : 0;
        }

        int l = as_int(eval_expr(b->left.get()));
        int r = as_int(eval_expr(b->right.get()));

        if (b->op == "+") return l + r;
        if (b->op == "-") return l - r;
        if (b->op == "*") return l * r;
        if (b->op == "/") return l / r;

        if (b->op == "==") return l == r;
        if (b->op == "!=") return l != r;
        if (b->op == "<") return l < r;
        if (b->op == "<=") return l <= r;
        if (b->op == ">") return l > r;
        if (b->op == ">=") return l >= r;

        throw std::runtime_error("Unknown operator: " + b->op);
    }

    else if (auto a = dynamic_cast<const assign_expr*>(e)) {
        value val = eval_expr(a->value.get());
        set_variable(a->name, val);
        return val;
    }

    else if (auto arr = dynamic_cast<const array_literal*>(e)) {
        std::vector<int> result;

        for (const auto& el : arr->elements) {
            result.push_back(as_int(eval_expr(el.get())));
        }

        return result;
    }

    else if (auto acc = dynamic_cast<const array_access*>(e)) {
        value v = get_variable(acc->name);

        if (!std::holds_alternative<std::vector<int>>(v)) {
            throw std::runtime_error("Not an array: " + acc->name);
        }

        int idx = as_int(eval_expr(acc->index.get()));
        const auto& arr = std::get<std::vector<int>>(v);

        if (idx < 0 || idx >= (int)arr.size()) {
            throw std::runtime_error("Index out of bounds");
        }

        return arr[idx];
    }

    throw std::runtime_error("Unknown expression type");
}

// =======================
// DEBUG
// =======================

void interpreter::dump_variables() const {
    std::cout << "\n=== GLOBAL VARIABLES ===\n";

    for (const auto& [k, v] : scopes.front()) {

        std::cout << k << " = ";

        if (std::holds_alternative<int>(v)) {
            std::cout << std::get<int>(v);
        } else {
            const auto& arr = std::get<std::vector<int>>(v);

            std::cout << "[";
            for (size_t i = 0; i < arr.size(); ++i) {
                std::cout << arr[i];
                if (i + 1 < arr.size()) std::cout << ", ";
            }
            std::cout << "]";
        }

        std::cout << "\n";
    }

    std::cout << "========================\n";
}