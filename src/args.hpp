#ifndef _GVC_H_ARGS_
#define _GVC_H_ARGS_

#include <iostream>
#include <map>

#include "IO.hpp"

namespace Gvc {
namespace Arguments {

const char NAMED_ARG_PREFIX = '-';

namespace ARG_TYPE {
    const char* IN_PATH = "-c";
    const char* OUT_PATH = "-o";
    const char* HELP = "-h";
    const char* VERSION = "-v";

    const char* VERBOSE = "--verbose";
}

std::string main_arg = "";
std::map<std::string, std::string> ArgumentsMap = {};

/**
 * @brief Checks if passed argument starts with '-'
 * 
 * @param arg passed argument
 * @return true if contains '-'
 * @return false if not
 */
bool _isArgType(std::string arg) {
    if (arg.length() < 2) return false;
    if (arg[0]!='\0' && arg[1]!='\0') {
        if (arg[0]==NAMED_ARG_PREFIX) return true;
    }
    return false;
}

/**
 * @brief Sets the main argument deciding which branch gvc will take.
 * If main arg has already been set, will error.
 * 
 * @param arg from ARG_TYPE
 */
void _setMainArg(std::string arg) {
    if (main_arg == "") {
        main_arg = arg;
    } else {
        IO::errCritical(std::string("Cannot set main arg ") + arg
            + " as it is already set to " + main_arg + "\n");
    }
}

/**
 * @brief Pushes an argument (without value) to ArgumentsMap
 * 
 * @param arg from ARG_TYPE
 */
void _pushArg(std::string arg) {
    ArgumentsMap.insert_or_assign(arg, "");
}

/**
 * @brief Pushes argument + following value to ArgumentsMap
 * 
 * @param argtype from ARG_TYPE
 * @param next errors if nullptr
 */
void _pushArgAndVal(std::string argtype, char* next) {
    if (next == nullptr) {
        IO::errCritical(
            std::string("Expected value after argument ") + argtype + "\n");
    }
    if (_isArgType(next)) {
        IO::errCritical(
            std::string("Expected value after argument ") + argtype + "\n");
    }
    ArgumentsMap.insert_or_assign(argtype, std::string(next));
}

/**
 * @brief Tries to match the passed argument with availaible argument types.
 * 
 * @param arg passed argument
 * @param next next passed argument, must be nullptr if no next exists
 */
void _matchArg(char* arg, char* next) {
    std::string s = arg;

    if (s == ARG_TYPE::HELP) {
        _setMainArg(ARG_TYPE::HELP);
        _pushArg(ARG_TYPE::HELP);
    }
    else
    if (s == ARG_TYPE::VERSION) {
        _setMainArg(ARG_TYPE::VERSION);
        _pushArg(ARG_TYPE::VERSION);
    }
    else
    if (s == ARG_TYPE::IN_PATH) {
        _setMainArg(ARG_TYPE::IN_PATH);
        _pushArgAndVal(s, next);
    }
    else
    if (s == ARG_TYPE::OUT_PATH) {
        _pushArgAndVal(s, next);
    }
    else
    if (s == ARG_TYPE::VERBOSE) {
        _pushArg(ARG_TYPE::VERBOSE);
        Gvc::Status::verbose_mode = true;
    }
}

std::string handleArgs(int size, char** args) {
    for(int i = 0; i<size; ++i) {
        _matchArg(args[i], (i+1<size) ? args[i+1] : nullptr);
    }
    // If no main argument has been passed, default to help
    if (main_arg == "") main_arg = "-h";
    return main_arg;
}

} /* namespace Arguments */
} /* namespace Gvc */

#endif // _GVC_H_ARGS_