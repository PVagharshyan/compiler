#include "executor.hpp"

#include "scope_tracker.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

#include <iostream>
#include <fstream>

void executor::run(const std::string& code) {

    tokens.clear();
    ast.clear();

    LOGGER.info("executed code: " + code, 2);

    // =========================
    // 1. LEXER PHASE
    // =========================

    lexer lex(code);
    scope_tracker scp_tracker;

    bool has_lex_error = false;

    while (true) {
        Token t = lex.nextToken();

        tokens.push_back(t);
        scp_tracker.process(t);

        if (t.type == TokenType::Unknown) {
            LOGGER.error("Unknown token: " + t.value);
            has_lex_error = true;
        }

        if (t.type == TokenType::EndOfFile)
            break;
    }

    if (scp_tracker.getLevel() != 0) {
        LOGGER.error("Unclosed scope detected.");
        has_lex_error = true;
    }

    LOGGER.info(printTokensToString(tokens), 2);

    exportTokensJSON(tokens, "tokens.json");

    // stop if lexer failed
    if (has_lex_error) {
        std::cout << "\nLog:\n";
        LOGGER.print();
        LOGGER.clear();
        return;
    }

    // =========================
    // 2. PARSER PHASE
    // =========================
    bool parse_ok = true;

    try {
        lexer lex2(code);
        parser p(lex2);

        ast = p.program();
    }
    catch (const std::exception& e) {
        LOGGER.error(std::string("Parser error: ") + e.what());
        parse_ok = false;
    }

    LOGGER.info(ast_to_string(ast), 2);

    exportASTJSON(ast, "ast.json");

    if (!parse_ok) {
        std::cout << "\nLog:\n";
        LOGGER.print();
        LOGGER.clear();
        return;
    }

    // =========================
    // 3. INTERPRETER PHASE
    // =========================

    try {
        interpreter interp;

        interp.run(ast);

        interp.dump_variables();

    }
    catch (const std::exception& e) {
        LOGGER.error(std::string("Runtime error: ") + e.what());
    }

    // =========================
    // OUTPUT LOGS
    // =========================

    std::cout << "\nLog:\n";
    LOGGER.print();
    LOGGER.clear();
}