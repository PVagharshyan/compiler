#include "lexer.hpp"

#include <string>

lexer::lexer(const std::string& s) : src(s) {
    //
}

char lexer::peek() const {
    return i < src.size() ? src[i] : '\0';
}

char lexer::get() {
    return i < src.size() ? src[i++] : '\0';
}

void lexer::skipSpaces() {
    while (isspace(peek())) get();
}

bool lexer::isKeyword(const std::string& w) const {
    return w == "if" || w == "else" || w == "while" || w == "for";
}

Token lexer::nextToken() {
    skipSpaces();
    char c = peek();

    if (c == '\0')
        return {TokenType::EndOfFile, ""};

    // IDENT / KEYWORD
    if (isalpha(c) || c == '_') {
        std::string w;
        while (isalnum(peek()) || peek() == '_')
            w += get();

        if (w == "if") return {TokenType::If, w};
        if (w == "else") return {TokenType::Else, w};
        if (w == "while") return {TokenType::While, w};
        if (w == "for") return {TokenType::For, w};

        return {TokenType::Identifier, w};
    }

    // NUMBER
    if (isdigit(c)) {
        std::string n;
        while (isdigit(peek()))
            n += get();

        return {TokenType::Number, n};
    }

    // SYMBOLS
    switch (get()) {
        case '{': return {TokenType::LBrace, "{"};
        case '}': return {TokenType::RBrace, "}"};
        case '(': return {TokenType::LParen, "("};
        case ')': return {TokenType::RParen, ")"};
        case ';': return {TokenType::Semicolon, ";"};
        case '=': return {TokenType::Assign, "="};
        case '+': case '-': case '*': case '/':
            return {TokenType::Operator, std::string(1, c)};
    }

    return {TokenType::Unknown, std::string(1, get())};
}