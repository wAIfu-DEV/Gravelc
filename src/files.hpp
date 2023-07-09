#ifndef _GVC_H_FILES_
#define _GVC_H_FILES_

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#include "IO.hpp"
#include "gravel/Token.hpp"

namespace Gvc {
namespace Files {

typedef std::vector<
    std::pair<std::string, std::string>
> FilesMap;

typedef std::vector<
    std::pair<std::string, std::vector<Token>>
> TokensMap;

FilesMap files_map = {};
TokensMap tokens_map = {};

void addFile(std::string path) {

    std::string content = "";

    std::fstream f (path);

    if (f.is_open()) {
        auto file_size = f.tellg(); 
        content.reserve(file_size);

        f.seekg(0);
        while (!f.eof()) {
            content += f.get();
        }
        f.close();
    } else {
        IO::errCritical(s("Failed to open file at given path ") + path
            + "\nPlease make sure it exists and is not open in another application.\n");
    }

    files_map.push_back(
        std::pair<std::string, std::string>(path, content));
    
    IO::log(s("added file ") + IO::COLOR_FG::CYAN + path
        + IO::COLOR_FG::GRAY + " to the file map.\n");
    IO::log(s("file contents:\n") + IO::COLOR_FG::CYAN + content + "\n");
}

std::string* getContent(const std::string& path) {
    for (size_t i = 0; i < files_map.size(); ++i) {
        if (files_map[i].first == path)
            return &files_map[i].second;
    }
    return nullptr;
}

std::vector<Token> getTokens(const std::string& path) {
    for (size_t i = 0; i < tokens_map.size(); ++i) {
        if (tokens_map[i].first == path)
            return tokens_map[i].second;
    }
    return {};
}

} // namespace Files
} // namespace Gvc

#endif // _GVC_H_FILES_