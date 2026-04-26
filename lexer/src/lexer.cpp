#include "lexer.hpp"

#include <cctype>
#include <unordered_map>

// -------------------- KEYWORDS --------------------
static const std::unordered_map<std::string, TokenType> keywords = {
    {"if", TokenType::If},
    {"else", TokenType::Else},
    {"while", TokenType::While},
    {"for", TokenType::For},
    {"integral", TokenType::Integral}
};

// -------------------- OPERATOR AUTOMATON --------------------
lexer::OpNode* lexer::opRoot = nullptr;

// add operator into trie
void lexer::addOperator(const std::string& op, TokenType type) {
    OpNode* cur = opRoot;

    for (char c : op) {
        if (!cur->next[c])
            cur->next[c] = new OpNode();

        cur = cur->next[c];
    }

    cur->token = type;
}

// build automaton once
void lexer::initOperatorAutomaton() {
    if (opRoot) return;

    opRoot = new OpNode();

    // ---- define operators here ONLY ----
    addOperator("+", TokenType::Operator);
    addOperator("-", TokenType::Operator);
    addOperator("*", TokenType::Operator);
    addOperator("/", TokenType::Operator);

    addOperator("=", TokenType::Assign);
    addOperator("==", TokenType::Operator);
    addOperator("!=", TokenType::Operator);

    addOperator("<", TokenType::Operator);
    addOperator("<=", TokenType::Operator);
    addOperator(">", TokenType::Operator);
    addOperator(">=", TokenType::Operator);

    addOperator("&&", TokenType::Operator);
    addOperator("||", TokenType::Operator);
}

// -------------------- CTOR --------------------
lexer::lexer(const std::string& s) : src(s) {
    initOperatorAutomaton();
}

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

// -------------------- OPERATOR / SYMBOL (DFA) --------------------
Token lexer::readOperatorOrSymbol() {
    char c = peek();

    // ---- fixed single-char symbols ----
    switch (c) {
        case '{': get(); return {TokenType::LBrace, "{"};
        case '}': get(); return {TokenType::RBrace, "}"};
        case '(': get(); return {TokenType::LParen, "("};
        case ')': get(); return {TokenType::RParen, ")"};
        case ';': get(); return {TokenType::Semicolon, ";"};
        case '[': get(); return {TokenType::LBracket, "["};
        case ']': get(); return {TokenType::RBracket, "]"};
        case ',': get(); return {TokenType::Comma, ","};
    }

    // ---- DFA traversal ----
    OpNode* cur = opRoot;
    OpNode* lastAccept = nullptr;

    size_t start = i;
    size_t lastPos = i;

    while (true) {
        char ch = peek();

        if (!cur->next.count(ch))
            break;

        cur = cur->next[ch];
        get();

        if (cur->token) {
            lastAccept = cur;
            lastPos = i;
        }
    }

    if (lastAccept) {
        i = lastPos;
        return {
            *(lastAccept->token),
            src.substr(start, lastPos - start)
        };
    }

    // ---- fallback ----
    return {TokenType::Unknown, std::string(1, get())};
}