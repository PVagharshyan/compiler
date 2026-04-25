#include "executor.hpp"
#include "logger.hpp"

#include <vector>
#include <string>

int main() {

    std::vector<std::string> tests = {
        "integral b = 20; integral a = [1,2,3]; integral x = 1; integral y = 1; y = a[1]; a[1] = b; a[2] = b + 10 + 3*(10 + a[1]); if (x == 10) { y = x + 5; } else { y = 0; }"

        ,"integral n = 5;\
integral i = 0;\
integral sum = 0;\
integral arr = [1, 2, 3, 4, 5];\
\
for (i = 0; i < n; i = i + 1) {\
    arr[i] = arr[i] * 2;\
}\
\
i = 0;\
while (i < n) {\
    sum = sum + arr[i];\
    i = i + 1;\
}\
\
if (sum > 20) {\
    sum = sum - 10;\
\
    if (sum > 15) {\
        sum = sum + 5;\
    } else {\
        sum = sum + 1;\
    }\
\
} else {\
    sum = 0;\
}\
\
arr[2] = sum;\
\
integral result = arr[0] + arr[1] + arr[2];"
        , "integral i = 1; integral arr = [1,2,3]; for (i = 0; i < 1; a[i] = 20) {i = i + 1;}"
        // ,"\
        // i = 1;\
        // sum = 0;\
        // while (i < 10) {\
        //     if (i == 5) {\
        //         sum = sum + i;\
        //     }\
        //     i =  i + 1;\
        // }\
        // "

        // ,"\
        // _var1 = 10;\
        // __hidden__ = _var1 + 20;\
        // a123b = __hidden__;\
        // "

        // ,"___ = 1; _a_b_c = ___ + 2; a1b2c3 = _a_b_c;"

        // ,"\
        // a = 10;\
        // b = 20;\
        // result_1 = 0;\
        // result_2 = 0;\
        // result_3 = 0;\
        // result_4 = 0;\
        // result_5 = 0;\
        // result_6 = 0;\
        // if (a==b) {\
        //     result_1 = 1;\
        // }\
        // if (a!=b) {\
        //     result_2 = 2;\
        // }\
        // if (a<=b) {\
        //     result_3 = 3;\
        // }\
        // if (a>=b) {\
        //     result_4 = 4;\
        // }\
        // if (a<b) {\
        //     if (result_2 == 2) {\
        //         result_5 = 2;\
        //     }\
        //     else {\
        //         result_5 = 5;\
        //     }\
        // }\
        // if (a>b) {\
        //     result_6 = 6;\
        // }\
        // "

        // ,"\
        // a = 1;\
        // b = 10;\
        // c = 10;\
        // d = 1;\
        // e = 1;\
        // x = a + b * c - d / e;\
        // "

        // ,"a && b || c; x <<= 2; y >>= 1; z += 5;"

        // ,"if(1) {a = 10;} if (a == 10) {b = 20;}"
        // // -------------------- LONGEST MATCH TEST --------------------
        // ,"a === b; x <<= 3; y >>= 2;"   // should test max munch

        // // -------------------- NUMBERS --------------------
        // ,"x = 0; y = 123456; z = 0001;"

        // // -------------------- WHITESPACE VARIATIONS --------------------
        // ,"   a=5;    b =10 ;   c= a+b ; "

        // ,"\t\na\t=\t5;\n\tb=10;\n"

        // // -------------------- NESTED STRUCTURES --------------------
        // ,"if (a) { if (b) { if (c) { x = 1; } } }"

        // ,"while (x < 10) { x = x + 1; if (x == 5) { break; } }"

        // // -------------------- INVALID TOKENS --------------------
        // ,"x = 10 @ 20; y = #invalid; z = 5 ^ 2;"

        // ,"a = 5 $ 3; b = 7 ~ 2;"

        // // -------------------- MIXED VALID + INVALID --------------------
        // ,"if (x == 10) { y = x @ 5; }"

        // // -------------------- EDGE CASES --------------------
        // ,""                         // empty input
        // ,"     "                    // only spaces
        // ,";"                        // only symbol
        // ,"{}"                       // braces only

        // // -------------------- IDENTIFIER EDGE CASES --------------------
        // ,"_ = 1;"
        // ,"__ = 2;"
        // ,"_1_2_3 = 10;"

        // // -------------------- OPERATOR EDGE CASES --------------------
        // ,"======"   // should tokenize as == == ==
        // ,"<<<<="    // depending on your operators

        // // -------------------- STRESS / LONG INPUT --------------------
        // ,"a=1; b=2; c=3; d=4; e=5; f=6; g=7; h=8; i=9; j=10;"

        // // -------------------- REALISTIC SNIPPET --------------------
        // ,"sum = 0; for (i = 0; i < 10; i = i + 1) { sum = sum + i; }"
    };

    executor exec;

    LOGGER.set_verbosity(2);

    for (const auto& code : tests) {
        exec.run(code);
    }

    return 0;
}