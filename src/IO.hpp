#ifndef _GVC_H_IO_
#define _GVC_H_IO_

#include <iostream>
#include <string>
#include <stdio.h>
#include <filesystem>

#include "status.hpp"

namespace Gvc {
namespace IO {

namespace COLOR_FG {
    const char* RESET = "\033[0m";
    const char* DEFAULT = "\033[0;37m";
    const char* GRAY = "\033[1;30m";
    const char* WHITE = "\033[1;37m";
    const char* YELLOW = "\033[0;33m";
    const char* RED = "\033[1;31m";
    const char* CYAN = "\033[1;36m";
} // namespace COLOR_FG

char inputc() {
    return getchar();
}

void print(const std::string &s) {
    std::cout << s;
    //std::cout.flush();
}

void log(const std::string &s) {
    if (Gvc::Status::verbose_mode) {
        std::cout << COLOR_FG::GRAY << s << COLOR_FG::RESET;
        //std::cout.flush();
    }
}

void warnPrint(const std::string &s) {
    std::cout << COLOR_FG::YELLOW << s << COLOR_FG::RESET;
    //std::cout.flush();
}

void errPrint(const std::string &s) {
    std::cout << COLOR_FG::RED << s << COLOR_FG::RESET;
    //std::cout.flush();
}

std::string fileLoc(const std::string& file_path, uint64_t line, uint64_t col) {
    std::filesystem::path p (file_path);
    return p.filename().generic_string() + ":" + std::to_string(line)
        + ":" + std::to_string(col);
}

void errAt(
    const std::string& msg,
    const std::string& file_path,
    uint64_t line, uint64_t col)
{
    std::cout << COLOR_FG::RESET << fileLoc(file_path, line, col) << ": "
        << COLOR_FG::RED << msg << COLOR_FG::RESET;
    //std::cout.flush();
}

void holdExit(int code) {
    /*print(std::string("Press ") + "\033[0;30m" + "\033[107m" + "ENTER"
        + "\033[0m" + " to exit . . .\n");
    inputc();*/
    exit(code);
}

void errCritical(const std::string &errmsg) {
    errPrint(errmsg);
    holdExit(1);
}

} // namespace IO
} // namespace Gvc

#endif // _GVC_H_IO_