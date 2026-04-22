#include "utils.hpp"

#include <iostream>

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::If: return "If";
        case TokenType::Else: return "Else";
        case TokenType::While: return "While";
        case TokenType::For: return "For";

        case TokenType::Identifier: return "Identifier";
        case TokenType::Number: return "Number";

        case TokenType::Operator: return "Operator";
        case TokenType::Assign: return "Assign";

        case TokenType::LBrace: return "LBrace";
        case TokenType::RBrace: return "RBrace";
        case TokenType::LParen: return "LParen";
        case TokenType::RParen: return "RParen";
        case TokenType::Semicolon: return "Semicolon";

        case TokenType::EndOfFile: return "EOF";
        case TokenType::Unknown: return "Unknown";
    }
    return "???";
}

std::string printTokensToString(const std::vector<Token>& tokens) {
    std::ostringstream oss;

    oss << "\nTokens:\n";
    oss << "----------------------------------------\n";
    oss << std::left
        << std::setw(5)  << "Idx"
        << std::setw(15) << "Type"
        << std::setw(15) << "Value"
        << "\n";
    oss << "----------------------------------------\n";

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& t = tokens[i];

        oss << std::left
            << std::setw(5)  << i
            << std::setw(15) << tokenTypeToString(t.type)
            << std::setw(15) << ("\"" + t.value + "\"")
            << "\n";
    }

    oss << "----------------------------------------\n";

    return oss.str();
}