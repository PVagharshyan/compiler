#pragma once

#include "token_types.hpp"
#include "grammar.hpp"

#include <vector>
#include <iomanip>
#include <memory>

std::string tokenTypeToString(TokenType type);

std::string printTokensToString(const std::vector<Token>& tokens);

std::string ast_to_string(const std::vector<std::unique_ptr<stmt>>& ast);