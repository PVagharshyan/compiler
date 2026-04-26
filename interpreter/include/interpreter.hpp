#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <variant>

#include "grammar.hpp"

using value = std::variant<int, std::vector<int>>;

class interpreter {
private:
    std::vector<std::unordered_map<std::string, value>> scopes;

    // ---------- scope ----------
    void push_scope();
    void pop_scope();

    // ---------- variable ----------
    value get_variable(const std::string& name);
    void set_variable(const std::string& name, const value& val);

    // ---------- helpers ----------
    int as_int(const value& v);

    // ---------- execution ----------
    void exec_stmt(const stmt* s);
    void exec_block(const std::vector<std::unique_ptr<stmt>>& block);

    value eval_expr(const expr* e);

    void set_array_element(const std::string& name, int index, int value);

public:
    void run(const std::vector<std::unique_ptr<stmt>>& ast);

    void dump_variables() const;
};