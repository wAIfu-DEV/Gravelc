#ifndef _GVC_H_PREPROCESSOR_
#define _GVC_H_PREPROCESSOR_

#include <iostream>
#include <regex>
#include <vector>
#include <map>

#include "../files.hpp"
#include "lexer.hpp"
#include "Token.hpp"

namespace Gvc {
namespace GravelPreProcessor {

typedef std::map<std::string, std::vector<std::string>> DependencyMap;
DependencyMap dependency_map= {};


typedef std::vector<std::string> DependencyStack;
DependencyStack dependency_stack = {};

std::string input_file_path = "";
bool has_errored = false;

void _recursiveDependencyPush(const std::string& path) {
    size_t found_index = 0;
    size_t stack_size = dependency_stack.size();
    while(found_index < stack_size) {
        if (dependency_stack[found_index] == path) break;
        ++found_index;
    }
    if (found_index == stack_size - 1) return;
    IO::log(s("push back dependency: ") + dependency_stack[found_index] + "\n");
    dependency_stack.erase(dependency_stack.begin() + found_index);
    if (dependency_map.count(path) > 0) {
        for (auto s : dependency_map[path]) {
            _recursiveDependencyPush(s);
        }
    } 
}

void _recursiveDependencyCheck(
    const std::string path,
    const std::string parent,
    const std::string origin)
{
    if (path == parent) {
        IO::errCritical("Found circular dependency to: " + origin + " from: " + parent + "\n");
    }
    if (dependency_map.count(path) > 0) {
        for (auto s : dependency_map[path]) {
            _recursiveDependencyCheck(s, parent, origin);
        }
    } 
}

void pushDependency(const std::string& path, const std::string& from) {
    _recursiveDependencyCheck(path, from, path);
    dependency_stack.push_back(path);
    if (from != "")
        dependency_map[from].push_back(path);
    _recursiveDependencyPush(path);
}

void _preprocess(const std::string& path, const std::string& from) {
    pushDependency(path, from);
    std::vector<Token::Tok> tokens = Files::getTokens(path);

    std::vector<std::string> included_files = {};

    // Get all preproc directives
    size_t TOKENS_SIZE = tokens.size();
    for (size_t i = 0; i < TOKENS_SIZE; ++i) {
        Token::Tok dir_token = tokens[i];

        // Link directive
        if (dir_token.type == Token::TOK_TYPE::KEYW_LINK) {
            // Bounds checking
            if (i + 1 >= TOKENS_SIZE) {
                IO::errAt("Expected string containing path or library name after link directive\n",
                    path, dir_token.line, dir_token.col);
                has_errored = true;
                continue;
            }
            Token::Tok path_token = tokens[i+1];

            if (path_token.type != Token::TOK_TYPE::STRING) {
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

    std::vector<std::string> dependencies_vector = {};
    // Lex all files then preprocess them
    for (std::string include_path : included_files) {

        // Including file
        if (include_path.find(".") != std::string::npos) {
            std::filesystem::path in_path = input_file_path;
            std::filesystem::current_path(in_path.parent_path());
            std::string abs_path = std::filesystem::absolute(include_path)
                                    .generic_string();
            dependencies_vector.push_back(abs_path);
            Files::addFile(abs_path);
            GravelLexer::startLexing(abs_path);
            _preprocess(abs_path, path);
            IO::log(s("linked to file ") + include_path + "\n");
        } 
        else { // Including library
            std::filesystem::current_path(Status::exe_directory);
            if (include_path.find(":") != std::string::npos) {
                std::string lib = include_path.substr(0, include_path.find(':'));
                std::string sublib = include_path.substr(include_path.find(':') + 1);
                std::string libpath = "lib/" + lib + "/" + sublib + ".gv";
                std::string abs_path = std::filesystem::absolute(libpath).generic_string();
                dependencies_vector.push_back(abs_path);
                Files::addFile(abs_path);
                GravelLexer::startLexing(abs_path);
                _preprocess(abs_path, path);
            } else {
                std::string libpath = "lib/" + include_path + "/" + include_path + ".gv";
                std::string abs_path = std::filesystem::absolute(libpath).generic_string();
                dependencies_vector.push_back(abs_path);
                Files::addFile(abs_path);
                GravelLexer::startLexing(abs_path);
                _preprocess(abs_path, path);
            }
            IO::log(s("linked to library ") + include_path + "\n");
        }
    }
    dependency_map[path] = dependencies_vector;

    if (has_errored) {
        IO::errCritical("There was an error during the preprocessing of the files.\n");
    }
}

/**
 * @brief Starts the pprocessing process using the input file as starting point.
 * 
 * @param path absolute path to the file
 */
void startPreprocess(const std::string& path) {
    input_file_path = path;
    _preprocess(path, "");

    IO::log("dep stack size: " + std::to_string(dependency_stack.size()) + "\n");
    IO::log("printing dependency stack . . .\n");
    for(std::string s : dependency_stack) {
        IO::log(s + "\n");
    }
}

} // namespace GravelPreProcessor
} // namespace Gvc


#endif