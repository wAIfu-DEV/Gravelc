#ifndef _GVC_
#define _GVC_

#ifdef _WIN64
    #define GVC_ARCH "x86_64-win64"
#elif _WIN32
    #define GVC_ARCH "x86_32-win32"
#elif __linux__
    #if __x86_64__
        #define GVC_ARCH "x86_64-linux"
    #else
        #define GVC_ARCH "x86_32-linux"
    #endif
#else
    #define GVC_ARCH "unknown"
#endif

#define GVC_VERS_CYCLE "dev"
#define GVC_VERS_MAJOR "1"
#define GVC_VERS_MINOR "0"
#define GVC_VERS_REVIS "1"
#define GVC_VERSION \
            GVC_ARCH " - " \
            GVC_VERS_CYCLE " " \
            GVC_VERS_MAJOR "." \
            GVC_VERS_MINOR "." \
            GVC_VERS_REVIS

#include <iostream>
#include <cstdint>

constexpr std::string __macro_to_str(const char* x) {return std::string(x);}
#define s(x) __macro_to_str(x)

#include "status.hpp"
#include "IO.hpp"
#include "files.hpp"
#include "args.hpp"
#include "help.hpp"
#include "gravel/Token.hpp"
#include "gravel/AstNode.hpp"
#include "gravel/rules.hpp"
#include "gravel/lexer.hpp"
#include "gravel/preprocessor.hpp"
#include "compile.hpp"
#include "flow.hpp"

int main(int argc, char **argv) {
    Gvc::Status::cwd = std::filesystem::current_path().generic_string();
    std::filesystem::path exe_path = argv[0];
    Gvc::Status::exe_directory = exe_path.parent_path().generic_string();

    std::string main_arg = Gvc::Arguments::handleArgs(argc, argv);
    Gvc::Flow::branch(main_arg);

    Gvc::IO::holdExit(0);
    return 0;
}

/*
CODING STYLE
functions/methods: camelCase            void exampleFunction() { ...
variables/arguments: snake_case         int my_int = 0;
constant: Captilized SNAKE_CASE         const int MY_CONST = 256;
private anything: _ prefix              int _private_var = 0;
heap allocated: $ prefix                char* $buff = malloc(... 
*/

#endif // _GVC_