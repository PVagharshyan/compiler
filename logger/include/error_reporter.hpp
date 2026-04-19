#pragma once

#include "error_types.hpp"

#include <vector>
#include <iostream>

class error_reporter {
    std::vector<Error> errors;

public:
    void report(ErrorType, const std::string&, int, int);

    bool hasErrors() const;

    void print() const;
};