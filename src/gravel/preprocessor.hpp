#ifndef _GVC_H_PREPROCESSOR_
#define _GVC_H_PREPROCESSOR_

#include <iostream>
#include <regex>

#include "../files.hpp"
#include "lexer.hpp"
#include "Token.hpp"

namespace Gvc {
namespace GravelPreProcessor {

std::string input_file_path = "";
bool has_errored = false;

void _preprocess(const std::string& path) {
    std::vector<Token> tokens = Files::getTokens(path);

    std::vector<std::string> included_files = {};

    // Get all preproc directives
    size_t TOKENS_SIZE = tokens.size();
    for (size_t i = 0; i < TOKENS_SIZE; ++i) {

        Token dir_token = tokens.at(i);

        // Link directive
        if (dir_token.type == TOK_TYPE::KEYW_LINK) {
            // Bounds checking
            if (i + 1 >= TOKENS_SIZE) {
                IO::errAt("Expected string containing path or library name after link directive\n",
                    path, dir_token.line, dir_token.col);
                has_errored = true;
                continue;
            }
            Token path_token = tokens.at(i+1);
            // If followed by wrong token
            if (path_token.type != TOK_TYPE::STRING) {
                IO::errAt("Expected string containing path or library name after link directive\n",
                    path, path_token.line, path_token.col);
                has_errored = true;
                continue;
            } else { [[likely]] // All good
                IO::log("found link directive to " + path_token.content + "\n");
                included_files.push_back(path_token.content);
            }
        }
    }

    // Lex all files then preprocess them
    for (std::string s : included_files) {

        // Including file
        if (s.find(".") != std::string::npos) {
            std::filesystem::path in_path = input_file_path;
            std::filesystem::current_path(in_path.parent_path());
            std::string abs_path = std::filesystem::absolute(s).generic_string();
            Files::addFile(abs_path);
            GravelLexer::startLexing(abs_path);
            _preprocess(abs_path);
            IO::log(s("linked to file ") + s + "\n");
        } 
        else { // Including library
            std::filesystem::current_path(Status::exe);
            if (s.find(":") != std::string::npos) {
                std::string lib = s.substr(0, s.find(':'));
                std::string sublib = s.substr(s.find(':') + 1);
                std::string libpath = "lib/" + lib + "/" + sublib + ".gv";
                std::string abs_path = std::filesystem::absolute(libpath).generic_string();
                Files::addFile(abs_path);
                GravelLexer::startLexing(abs_path);
                _preprocess(abs_path);
            } else {
                std::string libpath = "lib/" + s + "/" + s + ".gv";
                std::string abs_path = std::filesystem::absolute(libpath).generic_string();
                Files::addFile(abs_path);
                GravelLexer::startLexing(abs_path);
                _preprocess(abs_path);
            }
            IO::log(s("linked to library ") + s + "\n");
        }
    }

    if (has_errored) {
        IO::errCritical("There was an error during the preprocessing of the files.\n");
    }
}

void startPreprocess(const std::string& path) {
    input_file_path = path;
    _preprocess(path);
}

} // namespace GravelPreProcessor
} // namespace Gvc


#endif