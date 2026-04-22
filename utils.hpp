#pragma once

#include "token_types.hpp"

#include <vector>
#include <iomanip>

std::string tokenTypeToString(TokenType type);

std::string printTokensToString(const std::vector<Token>& tokens);