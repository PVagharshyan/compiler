#pragma once

#include "grammar.hpp"
#include "token_types.hpp"

#include <vector>
#include <string>
#include <memory>

class executor {
private:
    std::vector<Token> tokens;

    std::vector<std::unique_ptr<stmt>> ast;

public:
    void run(const std::string& code);
};