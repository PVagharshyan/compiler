#include "executor.hpp"

#include "scope_tracker.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <iostream>

void executor::run(const std::string& code) {

    tokens.clear();
    ast.clear();

    LOGGER.info("executed code: " + code, 2);

    // =========================
    // 1. LEXER PHASE
    // =========================
    lexer lex(code);
    scope_tracker scp_tracker;

    while (true) {
        Token t = lex.nextToken();

        tokens.push_back(t);
        scp_tracker.process(t);

        if (t.type == TokenType::Unknown) {
            LOGGER.error("Unknown token: " + t.value);
        }

        if (t.type == TokenType::EndOfFile)
            break;
    }

    if (scp_tracker.getLevel() != 0) {
        LOGGER.error("Unclosed scope detected.");
    }

    LOGGER.info(printTokensToString(tokens), 2);

    // =========================
    // 2. PARSER PHASE (NEW)
    // =========================
    try {
        lexer lex2(code);   // fresh lexer for parser (important!)
        parser p(lex2);

        ast = p.program();
    }
    catch (const std::exception& e) {
        LOGGER.error(std::string("Parser error: ") + e.what());
    }

    // =========================
    // DEBUG OUTPUT
    // =========================

    std::cout << "\nLog:\n";
    LOGGER.print();
    LOGGER.clear();
}

const std::vector<Token>& executor::getTokens() const {
    return tokens;
}

const std::vector<std::unique_ptr<stmt>>& executor::getAST() const {
    return ast;
}