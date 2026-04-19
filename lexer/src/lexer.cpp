#include "lexer.hpp"

#include <cctype>
#include <unordered_map>

// -------------------- KEYWORDS --------------------
static const std::unordered_map<std::string, TokenType> keywords = {
    {"if", TokenType::If},
    {"else", TokenType::Else},
    {"while", TokenType::While},
    {"for", TokenType::For}
};

// -------------------- OPERATORS (EXTENSIBLE) --------------------
static const std::unordered_map<std::string, TokenType> operators = {
    {"+", TokenType::Operator},
    {"-", TokenType::Operator},
    {"*", TokenType::Operator},
    {"/", TokenType::Operator},

    {"=", TokenType::Assign},
    {"==", TokenType::Operator},
    {"!=", TokenType::Operator},

    {"<", TokenType::Operator},
    {"<=", TokenType::Operator},
    {">", TokenType::Operator},
    {">=", TokenType::Operator},

    // you can extend freely:
    // {"&&", TokenType::Operator},
    // {"||", TokenType::Operator},
    // {"<<=", TokenType::Operator},
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
    while (std::isspace(peek()))
        get();
}

// -------------------- MAIN DISPATCHER --------------------
Token lexer::nextToken() {
    skipSpaces();

    char c = peek();

    if (c == '\0')
        return {TokenType::EndOfFile, ""};

    if (std::isalpha(c) || c == '_')
        return readIdentifierOrKeyword();

    if (std::isdigit(c))
        return readNumber();

    return readOperatorOrSymbol();
}

// -------------------- IDENTIFIER / KEYWORD --------------------
Token lexer::readIdentifierOrKeyword() {
    std::string w;

    while (std::isalnum(peek()) || peek() == '_')
        w += get();

    auto it = keywords.find(w);
    if (it != keywords.end())
        return {it->second, w};

    return {TokenType::Identifier, w};
}

// -------------------- NUMBER --------------------
Token lexer::readNumber() {
    std::string n;

    while (std::isdigit(peek()))
        n += get();

    return {TokenType::Number, n};
}

// -------------------- OPERATOR / SYMBOL --------------------
Token lexer::readOperatorOrSymbol() {
    char c = peek();

    // ---- single char symbols (keep separate) ----
    switch (c) {
        case '{': get(); return {TokenType::LBrace, "{"};
        case '}': get(); return {TokenType::RBrace, "}"};
        case '(': get(); return {TokenType::LParen, "("};
        case ')': get(); return {TokenType::RParen, ")"};
        case ';': get(); return {TokenType::Semicolon, ";"};
    }

    // ---- longest match for operators ----
    std::string op;
    std::string bestMatch;

    size_t j = i;

    while (j < src.size()) {
        op += src[j];

        if (operators.count(op)) {
            bestMatch = op;
        }

        j++;
    }

    if (!bestMatch.empty()) {
        for (size_t k = 0; k < bestMatch.size(); ++k)
            get();

        return {operators.at(bestMatch), bestMatch};
    }

    // ---- fallback ----
    return {TokenType::Unknown, std::string(1, get())};
}