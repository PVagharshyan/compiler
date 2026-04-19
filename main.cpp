#include "lexer.hpp"
#include "scope_tracker.hpp"
#include "error_reporter.hpp"

#include <iostream>
#include <string>

int main() {
    std::string code =
        "if (a) { "
        "   while(b) { c = c + 1; } "
        "} else { for(i=0; i; i=i+1) {} }";

    lexer          lex(code);
    scope_tracker  scp_traker;
    error_reporter reporter;

    while (true) {
        Token t = lex.nextToken();
        scp_traker.process(t);

        if (t.type == TokenType::EndOfFile) {
            if (scp_traker.getLevel() != 0) {
                reporter.report(ErrorType::SemanticError, "Unclosed scope detected.", 0, 0);
            }
            reporter.print();
            break;
        }
    }
}