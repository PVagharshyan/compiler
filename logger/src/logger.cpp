#include "logger.hpp"
#include <iostream>

// ---------------- Singleton ----------------
logger& logger::instance() {
    static logger inst;
    return inst;
}

// ---------------- Core log ----------------
void logger::log(LogLevel level,
                 const std::string& msg,
                 int verbosity)
{
    if (verbosity < (int)requiredVerbosity)
        return;

    logs.push_back({level, msg});
}

// ---------------- Convenience API ----------------
void logger::info(const std::string& msg, int verbosity) {
    log(LogLevel::Info, msg, verbosity);
}

void logger::warn(const std::string& msg, int verbosity) {
    log(LogLevel::Warning, msg, verbosity);
}

void logger::error(const std::string& msg, int verbosity) {
    log(LogLevel::Error, msg, verbosity);
}

// ---------------- Print ----------------
void logger::print() const {
    for (const auto& e : logs) {

        const char* levelStr =
            (e.level == LogLevel::Info) ? "INFO" :
            (e.level == LogLevel::Warning) ? "WARNING" : "ERROR";

        std::cout << "[" << levelStr << "] "
                  << e.message
                  << std::endl;
    }
}

// ---------------- Utils ----------------
void logger::clear() {
    logs.clear();
}

void logger::set_verbosity(unsigned int verbosity) {
    requiredVerbosity = verbosity;
}