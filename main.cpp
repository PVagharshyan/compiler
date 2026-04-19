#include "executor.hpp"
#include "logger.hpp"

#include <vector>
#include <string>

int main() {

    std::vector<std::string> tests = {
        "if (x == 10) { y = x + 5; } else { y = 0; }",

        "while (i < 10) { for (j = 0; j < i; j = j + 1) { if (j % 2 == 0) { sum = sum + j; } } }",

        "_var1 = 10; __hidden__ = _var1 + 20; a123b = __hidden__;",

        "a = 5; b = 10; if (a <= b) { c = a + b * 2 - 3 / 1; } if (a != b) { c = 0; }",

        "x = 10 @ 20; y = #invalid; z = 5 ^ 2;"
    };

    executor exec;

    LOGGER.set_verbosity(0);

    for (const auto& code : tests) {
        exec.run(code);
    }

    return 0;
}