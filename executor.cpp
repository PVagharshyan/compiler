#include "executor.hpp"

#include "scope_tracker.hpp"
#include "logger.hpp"
#include "utils.hpp"

#include <iostream>

void executor::run(const std::string& code) {

    tokens.clear();

    LOGGER.info("executed code:" + code, 2);

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

    std::string tockens_str = printTokensToString(tokens);
    LOGGER.info(tockens_str, 2);

    std::cout << "\nLogs:\n";
    LOGGER.print();
    LOGGER.clear();
}

const std::vector<Token>& executor::getTokens() const {
    return tokens;
}