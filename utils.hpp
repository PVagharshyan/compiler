#pragma once

#include "token_types.hpp"

#include <vector>
#include <iomanip>

std::string tokenTypeToString(TokenType type);

void printTokens(const std::vector<Token>& tokens);