#pragma once 

#include "token_types.hpp"

class scope_tracker {
    int scopeLevel = 0;

public:
    void process(const Token&);

    int getLevel() const;
};