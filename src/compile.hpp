#ifndef _GVC_H_COMPILE_
#define _GVC_H_COMPILE_

#include "args.hpp"
#include "IO.hpp"
#include "files.hpp"
#include "gravel/lexer.hpp"
#include "gravel/preprocessor.hpp"

namespace Gvc {
namespace Compiler {

std::string _input_path = "";
std::string _output_path = "";

void _initializeCompilation() {
    auto iter = Arguments::ArgumentsMap.find(Arguments::ARG_TYPE::OUT_PATH);
    // Found
    if (iter != Arguments::ArgumentsMap.end()) {
        IO::log("found output file path in arguments.\n");
        _output_path = Arguments::ArgumentsMap[Arguments::ARG_TYPE::OUT_PATH];
    }

    iter = Arguments::ArgumentsMap.find(Arguments::ARG_TYPE::IN_PATH);
    if (iter != Arguments::ArgumentsMap.end()) {
        _input_path = Arguments::ArgumentsMap[Arguments::ARG_TYPE::IN_PATH];
    } else {
        IO::errCritical("Could not read input file path from arguments.\n");
    }
    IO::log("passed requirements check.\n");
}

void startCompile() {
    _initializeCompilation();

    std::filesystem::path absolute_path = std::filesystem::absolute(_input_path);
    // Set current working directory to the parent folder of input file
    std::filesystem::current_path(absolute_path.parent_path());
    _input_path = absolute_path.generic_string();

    Files::addFile(_input_path);
    GravelLexer::startLexing(_input_path);
    GravelPreProcessor::startPreprocess(_input_path);
}

} // namespace Compiler
} // namespace Gvc


#endif // _GVC_H_COMPILE_