#ifndef _GVC_H_HELP_
#define _GVC_H_HELP_

#include "IO.hpp"

namespace Gvc {
namespace Help {

/**
 * @brief Shows a list of available arguments for gvc
 */
void displayHelp() {
    constexpr
    int64_t _HELPLINES_SIZE = 10;

    constexpr
    const char* _help_lines[_HELPLINES_SIZE] = {
    "-h              Displays a list of the arguments available in gvc.",
    "-v              Displays the architecture and version of gvc.",
    "-c [PATH]       Compiles the file at the given path. If no output path",
    "                 was given, the executable file will be dumped in the",
    "                 same folder.",
    "-o [PATH]       Specifies the path for the output file. PATH can be a",
    "                 file or a directory.",
    "--verbose       Displays a greater amount of logs during the execution of",
    "                 the process. Useful for debugging, but will impact",
    "                 performances.",
    };

    for (int64_t i = 0; i < _HELPLINES_SIZE; ++i) {
        IO::print(std::string(_help_lines[i]) + "\n");
    }
}

} // namespace Help
} // namespace Gvc

#endif // _GVC_H_HELP_
