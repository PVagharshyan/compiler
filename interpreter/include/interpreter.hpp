#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

#include "grammar.hpp"

class interpreter {
private:
    // scope stack: inner → outer
    std::vector<std::unordered_map<std::string, int>> scopes;

    bool break_flag = false;

    // --- scope helpers ---
    void push_scope();
    void pop_scope();

    int get_variable(const std::string& name);
    void set_variable(const std::string& name, int value);

    // --- execution ---
    void exec_stmt(const stmt* s);
    void exec_block(const std::vector<std::unique_ptr<stmt>>& block);

    int eval_expr(const expr* e);

public:
    void run(const std::vector<std::unique_ptr<stmt>>& ast);

    void dump_variables() const;
};