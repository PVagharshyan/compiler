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
        
        //A sorting algorithm implemented in a new language.(Bubble sort)
        ,"integral n = 10;\
        integral arr = [9, 1, 8, 2, 7, 3, 6, 4, 5, 0];\
        integral temp = 0;\
        integral i = 0;\
        integral j = 0;\
        \
        for (i = 0; i < n; i = i + 1) {\
            for (j = 0; j < n - 1; j = j + 1) {\
                if (arr[j] > arr[j + 1]) {\
                    temp = arr[j];\
                    arr[j] = arr[j + 1];\
                    arr[j + 1] = temp;\
                } else {\
                    temp = 0;\
                }\
            }\
        }\
        \
        integral sum = 0;\
        i = 0;\
        \
        while (i < n) {\
            sum = sum + arr[i];\
        \
            if (arr[i] > 5) {\
                sum = sum + 10;\
            } else {\
                sum = sum + 1;\
            }\
        \
            i = i + 1;\
        }\
        \
        integral max = arr[0];\
        i = 1;\
        \
        while (i < n) {\
            if (arr[i] > max) {\
                max = arr[i];\
            }\
            i = i + 1;\
        }\
        \
        if (sum > max) {\
            sum = sum - max;\
        } else {\
            sum = sum + max;\
        }\
        \
        integral result = sum + max;\
        \
        if (result > 50) {\
            result = result / 2;\
        } else {\
            result = result + 5;\
        }"
    };

    executor exec;

    LOGGER.set_verbosity(2);

    for (const auto& code : tests) {
        exec.run(code);
    }

    return 0;
}