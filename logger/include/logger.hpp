#pragma once

#include "logger_types.hpp"

#include <vector>
#include <string>

class logger {
    std::vector<LogEntry> logs;

    unsigned int requiredVerbosity = 4;

    logger() = default;

public:
    logger(const logger&) = delete;
    logger& operator=(const logger&) = delete;

    static logger& instance();

    void info(const std::string& msg, int verbosity = 0);
    void warn(const std::string& msg, int verbosity = 4);
    void error(const std::string& msg, int verbosity = 5);

    void log(LogLevel level, const std::string& msg, int verbosity);

    void print() const;
    void clear();

    void set_verbosity(unsigned int verbosity);
};

#define LOGGER logger::instance()