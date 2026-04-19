#pragma once

#include "lexer.hpp"
#include <vector>
#include <string>

class executor {
    std::vector<Token> tokens;

public:
    void run(const std::string& code);

    const std::vector<Token>& getTokens() const;
};