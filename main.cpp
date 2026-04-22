#include "executor.hpp"
#include "logger.hpp"

#include <vector>
#include <string>

int main() {

    std::vector<std::string> tests = {
        // -------------------- BASIC CONTROL FLOW --------------------
        "if (x == 10) { y = x + 5; } else { y = 0; }",

        "while (i < 10) { for (j = 0; j < i; j = j + 1) { if (j % 2 == 0) { sum = sum + j; } } }",

        // -------------------- IDENTIFIERS --------------------
        "_var1 = 10; __hidden__ = _var1 + 20; a123b = __hidden__;",

        "___ = 1; _a_b_c = ___ + 2; a1b2c3 = _a_b_c;",

        // -------------------- OPERATORS (DFA TEST) --------------------
        "a==b; a!=b; a<=b; a>=b; a<b; a>b;",

        "x = a + b * c - d / e;",

        // if you later add these operators, they should work automatically
        "a && b || c; x <<= 2; y >>= 1; z += 5;",

        // -------------------- LONGEST MATCH TEST --------------------
        "a === b; x <<= 3; y >>= 2;",   // should test max munch

        // -------------------- NUMBERS --------------------
        "x = 0; y = 123456; z = 0001;",

        // -------------------- WHITESPACE VARIATIONS --------------------
        "   a=5;    b =10 ;   c= a+b ; ",

        "\t\na\t=\t5;\n\tb=10;\n",

        // -------------------- NESTED STRUCTURES --------------------
        "if (a) { if (b) { if (c) { x = 1; } } }",

        "while (x < 10) { x = x + 1; if (x == 5) { break; } }",

        // -------------------- INVALID TOKENS --------------------
        "x = 10 @ 20; y = #invalid; z = 5 ^ 2;",

        "a = 5 $ 3; b = 7 ~ 2;",

        // -------------------- MIXED VALID + INVALID --------------------
        "if (x == 10) { y = x @ 5; }",

        // -------------------- EDGE CASES --------------------
        "",                         // empty input
        "     ",                    // only spaces
        ";",                        // only symbol
        "{}",                       // braces only

        // -------------------- IDENTIFIER EDGE CASES --------------------
        "_ = 1;",
        "__ = 2;",
        "_1_2_3 = 10;",

        // -------------------- OPERATOR EDGE CASES --------------------
        "======",   // should tokenize as == == ==
        "<<<<=",    // depending on your operators

        // -------------------- STRESS / LONG INPUT --------------------
        "a=1; b=2; c=3; d=4; e=5; f=6; g=7; h=8; i=9; j=10;",

        // -------------------- REALISTIC SNIPPET --------------------
        "sum = 0; for (i = 0; i < 10; i = i + 1) { sum = sum + i; }"
    };

    executor exec;

    LOGGER.set_verbosity(3);

    for (const auto& code : tests) {
        exec.run(code);
    }

    return 0;
}