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
    std::pair<std::string, std::vector<Token::Tok>>
> TokensMap;

FilesMap files_map = {};
TokensMap tokens_map = {};

/**
 * @brief Adds the content of the file to Files::files_map.
 * 
 * @param path absolute path to the file, errors if bad path.
 */
void addFile(std::string path) {

    std::string content = "";
    std::fstream f (path);

    // Read file
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

    files_map.push_back({ path, content });
    
    IO::log(s("added file ") + IO::COLOR_FG::CYAN + path
        + IO::COLOR_FG::GRAY + " to the file map.\n");
    IO::log(s("file contents:\n") + IO::COLOR_FG::CYAN + content + "\n");
}

/**
 * @brief Get a pointer to the content of the wanted file if it has been added
 * using Files::addFile
 * @param path absolute path to the file
 * @return pointer to memory in the Files::files_map entry for the file.
 * DO NOT FREE!
 */
std::string* getContent(const std::string& path) {
    for (size_t i = 0; i < files_map.size(); ++i) {
        if (files_map[i].first == path)
            return &files_map[i].second;
    }
    return nullptr;
}

/**
 * @brief Returns a vector of Tokens from Files::tokens_map. If file wasn't
 * lexed, returns empty vector.
 * 
 * @param path absolute path to file
 * @return std::vector<Token> 
 */
std::vector<Token::Tok> getTokens(const std::string& path) {
    for (size_t i = 0; i < tokens_map.size(); ++i) {
        if (tokens_map[i].first == path)
            return tokens_map[i].second;
    }
    return {};
}

} // namespace Files
} // namespace Gvc

#endif // _GVC_H_FILES_