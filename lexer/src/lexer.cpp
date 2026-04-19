#include "lexer.hpp"

#include <cctype>
#include <string>
#include <unordered_map>

// -------------------- KEYWORDS --------------------
static const std::unordered_map<std::string, TokenType> keywords = {
    {"if", TokenType::If},
    {"else", TokenType::Else},
    {"while", TokenType::While},
    {"for", TokenType::For}
};

// -------------------- CTOR --------------------
lexer::lexer(const std::string& s) : src(s) {}

// -------------------- BASIC OPS --------------------
char lexer::peek() const {
    return i < src.size() ? src[i] : '\0';
}

char lexer::get() {
    return i < src.size() ? src[i++] : '\0';
}

void lexer::skipSpaces() {
    while (std::isspace(peek())) get();
}

// -------------------- TOKENIZER --------------------
Token lexer::nextToken() {
    skipSpaces();
    char c = peek();

    if (c == '\0')
        return {TokenType::EndOfFile, ""};

    // ---------- IDENTIFIER / KEYWORD ----------
    if (std::isalpha(c) || c == '_') {
        std::string w;
        while (std::isalnum(peek()) || peek() == '_')
            w += get();

        auto it = keywords.find(w);
        if (it != keywords.end())
            return {it->second, w};

        return {TokenType::Identifier, w};
    }

    // ---------- NUMBER ----------
    if (std::isdigit(c)) {
        std::string n;
        while (std::isdigit(peek()))
            n += get();

        return {TokenType::Number, n};
    }

    // ---------- OPERATORS / SYMBOLS ----------
    switch (c) {
        case '{': get(); return {TokenType::LBrace, "{"};
        case '}': get(); return {TokenType::RBrace, "}"};
        case '(': get(); return {TokenType::LParen, "("};
        case ')': get(); return {TokenType::RParen, ")"};
        case ';': get(); return {TokenType::Semicolon, ";"};

        case '+': case '-': case '*': case '/': {
            char op = get();
            return {TokenType::Operator, std::string(1, op)};
        }

        case '=': {
            get();
            if (peek() == '=') {
                get();
                return {TokenType::Operator, "=="};
            }
            return {TokenType::Assign, "="};
        }

        case '!': {
            get();
            if (peek() == '=') {
                get();
                return {TokenType::Operator, "!="};
            }
            return {TokenType::Unknown, "!"};
        }

        case '<': {
            get();
            if (peek() == '=') {
                get();
                return {TokenType::Operator, "<="};
            }
            return {TokenType::Operator, "<"};
        }

        case '>': {
            get();
            if (peek() == '=') {
                get();
                return {TokenType::Operator, ">="};
            }
            return {TokenType::Operator, ">"};
        }
    }

    // ---------- UNKNOWN ----------
    char unknown = get();
    return {TokenType::Unknown, std::string(1, unknown)};
}