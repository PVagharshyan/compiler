#include "scope_tracker.hpp"

void scope_tracker::process(const Token& t) {
    if (t.type == TokenType::LBrace)
        scopeLevel++;
    else if (t.type == TokenType::RBrace)
        scopeLevel--;
}

int scope_tracker::getLevel() const {
    return scopeLevel;
}