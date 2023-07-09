#ifndef _GVC_H_DISPATCH_
#define _GVC_H_DISPATCH_

#include <iostream>

#include "main.cpp"
#include "IO.hpp"
#include "args.hpp"
#include "help.hpp"
#include "compile.hpp"

namespace Gvc {
namespace Flow {

void branch(std::string main_arg) {
    if (main_arg == Arguments::ARG_TYPE::VERSION) {
        IO::log("branch: version\n");
        IO::print("gvc " GVC_VERSION "\n");
    }
    else
    if (main_arg == Arguments::ARG_TYPE::HELP) {
        IO::print("branch: help\n");
        Help::displayHelp();
    }
    else
    if (main_arg == Arguments::ARG_TYPE::IN_PATH) {
        IO::log("branch: compile\n");
        Compiler::startCompile();
    }
}

} // namespace Flow
} // namespace Gvc

#endif // _GVC_H_DISPATCH_
