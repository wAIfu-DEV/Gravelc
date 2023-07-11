#ifndef _GVC_H_LEXER_
#define _GVC_H_LEXER_

#include "../main.cpp"
#include "../IO.hpp"
#include "../files.hpp"
#include "Token.hpp"
#include "rules.hpp"

namespace Gvc {
namespace GravelLexer {

using namespace Token;

enum LEX_MODE {
    DEFAULT,
    STRING,
    CHAR,
    NUMBER,
    COMMENT,
};

// TODO: move this to a singleton class with initializer
namespace LexerStatus {
    std::string file_path = "";
    enum LEX_MODE lexing_mode = LEX_MODE::DEFAULT;
    bool has_errored = false;
    bool escape_next = false;
    uint16_t tok_line = 1;
    uint16_t tok_col = 1;
    uint16_t line = 1;
    uint16_t col = 1;
    uint64_t char_index = 0;
    std::string* data = nullptr;
    std::vector<Tok> tokens = {};
    std::vector<char> tok_buff = {};
}

/**
 * @brief Checks is is at end of file.
 * 
 * @return true 
 * @return false 
 */
bool isNotEOF() {
    return LexerStatus::char_index + 1 < LexerStatus::data->length();
}

/**
 * @brief Peaks the next character in the file without consuming it
 * 
 * @return char 
 */
char peakNext() {
    if (LexerStatus::char_index >= LexerStatus::data->length()) return '\0';
    return LexerStatus::data->at(LexerStatus::char_index);
}

/**
 * @brief Gets the next character in the file
 * 
 * @return char 
 */
char getNext() {
    char c = LexerStatus::data->at(LexerStatus::char_index);
    LexerStatus::char_index++;
    LexerStatus::col++;
    if (c == '\n') {
        LexerStatus::line++;
        LexerStatus::col = 1;
    }
    return c;
}

/**
 * @brief Pushed current token in LexerStatus::tok_buff to LexerStatus::tokens
 * 
 * @param type 
 */
void pushToken(enum TOK_TYPE type) {
    std::string s = std::string(
        LexerStatus::tok_buff.begin(), LexerStatus::tok_buff.end());

    if (s == "") return;

    Tok t = Tok();
    t.line = LexerStatus::tok_line;
    t.col = LexerStatus::tok_col;
    t.type = type;
    t.content = s;
    LexerStatus::tokens.push_back(t);
    LexerStatus::tok_buff.clear();
    LexerStatus::tok_line = LexerStatus::line;
    LexerStatus::tok_col = LexerStatus::col;
}

bool isAlphaNumeric(char c) {
    return ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)
            || c == 95 || c == 36);
}

bool isNumeric(char c) {
    return ((c >= 48 && c <= 57) || c == 46 || c == 45);
}

bool isHexNumeric(char c) {
    return ((c >= 48 && c <= 57) || (c > 65 && c < 70) || (c >= 97 && c <= 102));
}

bool isWhiteSpace(char c) {
    return (c >= 1 && c <= 32);
}

/**
 * @brief Tries to match the current char to an available token type
 * 
 * @return TOK_TYPE 
 */
TOK_TYPE tryMatchCharToken(char c) {
    // Check if is single char token or resered keyword
    for (size_t i = 0; i < GravelRules::TOK_LOOKUP_TABLE_SIZE; ++i) {
        if (std::string(1,c) == s(GravelRules::tok_lookup_table[i].first)) {
            return GravelRules::tok_lookup_table[i].second;
        }
    }
    return TOK_TYPE::NONE;
}

/**
 * @brief Tries to match the current token to an available token type
 * 
 * @return TOK_TYPE 
 */
TOK_TYPE tryMatchToken() {
    std::string tok = std::string(
        LexerStatus::tok_buff.begin(), LexerStatus::tok_buff.end());

    // Check if is single char token or resered keyword
    for (size_t i = 0; i < GravelRules::TOK_LOOKUP_TABLE_SIZE; ++i) {
        if (tok == GravelRules::tok_lookup_table[i].first) {
            return GravelRules::tok_lookup_table[i].second;
        }
    }
    
    // Check if hex number
    if (tok.starts_with("0x")) {
        for (size_t i = 2; i < tok.length(); ++i) {
            if (!isHexNumeric(tok[i])) break;
            if (i == tok.length() - 1) {
                return TOK_TYPE::HEX_NUMBER;
            }
        }
    }

    // Check if number
    for (size_t i = 0; i < tok.length(); ++i) {
        if (!isNumeric(tok[i])) break;
        if (i == tok.length() - 1) {
            return TOK_TYPE::NUMBER;
        }
    }

    // Check if name
    for (size_t i = 0; i < tok.length(); ++i) {
        if (!isAlphaNumeric(tok[i])) break;
        if (i == tok.length() - 1) {
            return TOK_TYPE::NAME;
        }
    }
    return TOK_TYPE::NONE;
}

/**
 * @brief Changes lex mode to LEX_MODE::COMMENT depending on current mode
 * 
 */
void switchOnComment() {
    if (LexerStatus::lexing_mode == LEX_MODE::STRING) {}
    else if (LexerStatus::lexing_mode == LEX_MODE::CHAR) {}
    else {
        pushToken(tryMatchToken());
        LexerStatus::lexing_mode = LEX_MODE::COMMENT;
    }
}

/**
 * @brief Changes lex mode to LEX_MODE::CHAR depending on current mode
 * 
 */
void switchOnCharQuote() {
    if (LexerStatus::lexing_mode == LEX_MODE::CHAR) {
        pushToken(TOK_TYPE::CHAR);
        LexerStatus::lexing_mode = LEX_MODE::DEFAULT;
    }
    else
    if (LexerStatus::lexing_mode == LEX_MODE::STRING) {}
    else {
        pushToken(tryMatchToken());
        LexerStatus::lexing_mode = LEX_MODE::CHAR;
    }
}

/**
 * @brief Changes lex mode to LEX_MODE::STRING depending on current mode
 * 
 */
void switchOnStringQuote() {
    if (LexerStatus::lexing_mode == LEX_MODE::STRING) {
        pushToken(TOK_TYPE::STRING);
        LexerStatus::lexing_mode = LEX_MODE::DEFAULT;
    }
    else
    if (LexerStatus::lexing_mode == LEX_MODE::CHAR) {}
    else {
        pushToken(tryMatchToken());
        LexerStatus::lexing_mode = LEX_MODE::STRING;
    }  
}

/**
 * @brief if is // and not escaped
 * 
 * @param c 
 * @return true 
 * @return false 
 */
bool isComment(char c) {
    if (LexerStatus::escape_next) return false;
    return (c == '/' && peakNext() == '/');
}

/**
 * @brief if is ` and not escaped
 * 
 * @param c 
 * @return true 
 * @return false 
 */
bool isCharQuote(char c) {
    if (LexerStatus::escape_next) return false;
    return (c == '`');
}

/**
 * @brief if is " or ' and not escaped
 * 
 * @param c 
 * @return true 
 * @return false 
 */
bool isStringQuote(char c) {
    if (LexerStatus::escape_next) return false;
    return (c == '"' || c == '\'');
}

/**
 * @brief Checks if tokens have correctly been interpreted
 * 
 */
void _verifyTokens() {
    IO::log("verifying tokens . . .\n");

    if (LexerStatus::tokens.size() == 0) {
        IO::log("input file was empty\n");
    }

    for (size_t i = 0; i < LexerStatus::tokens.size(); ++i) {
        Tok t = LexerStatus::tokens.at(i);
        if (t.type == TOK_TYPE::NONE) {
            IO::errAt(
                s("Could not understand token: ") + IO::COLOR_FG::YELLOW
                + t.content + "\n",
                LexerStatus::file_path, t.line, t.col);
            LexerStatus::has_errored = true;
        }
        IO::log(t.content + ":" + std::to_string(t.type) + "\n");
    }
}

/**
 * @brief Lexes the file and pushes the tokens to LexerStatus::tokens
 * 
 * @param file_path absolute path to the file
 * @param content_ptr pointer to the content of the file
 */
void _lex(const std::string& file_path, std::string* content_ptr) {
    IO::log(s("lexing ") + file_path + "\n");

    // Reinitialize state, there MUST be a better way to do it
    LexerStatus::file_path = file_path;
    LexerStatus::data = content_ptr;
    LexerStatus::lexing_mode = LEX_MODE::DEFAULT;
    LexerStatus::escape_next = false;
    LexerStatus::has_errored = false;
    LexerStatus::char_index = 0;
    LexerStatus::col = 1;
    LexerStatus::line = 1;
    LexerStatus::tok_col = 1;
    LexerStatus::tok_line = 1;
    LexerStatus::tok_buff = {};
    LexerStatus::tokens = {};

    while(isNotEOF()) {
        char c = getNext();

        if (isComment(c) && (
            LexerStatus::lexing_mode != LEX_MODE::STRING &&
            LexerStatus::lexing_mode != LEX_MODE::CHAR
        )) {
            switchOnComment();
            continue;
        }

        if (LexerStatus::lexing_mode == LEX_MODE::COMMENT) {
            if (c == '\n') LexerStatus::lexing_mode = LEX_MODE::DEFAULT;
            continue;
        }

        if (isCharQuote(c)) {
            switchOnCharQuote();
            continue;
        }   
        if (isStringQuote(c)) {
             switchOnStringQuote();
             continue;
        }

        if (LexerStatus::lexing_mode == LEX_MODE::STRING) {
            LexerStatus::tok_buff.push_back(c);
            continue;
        }

        // Try matching single-char tokens
        TOK_TYPE char_tok_type = tryMatchCharToken(c);
        if (char_tok_type != TOK_TYPE::NONE) {
            pushToken(tryMatchToken());
            LexerStatus::tok_buff.push_back(c);
            pushToken(char_tok_type);
            continue;
        }

        if (isWhiteSpace(c)) {
            pushToken(tryMatchToken());
            continue;
        }
        LexerStatus::tok_buff.push_back(c);
    }
}

/**
 * @brief Starts the lexing process using the input file as starting point.
 * 
 * @param path absolute path to the file
 */
void startLexing(const std::string& path) {
    std::string* file_content = Files::getContent(path);
    _lex(path, file_content);
    _verifyTokens();
    Files::tokens_map.push_back({ path, LexerStatus::tokens });
    if (LexerStatus::has_errored) {
        IO::errCritical("There was an error during the lexing process.\n");
    }
}

} // namespace GravelLexer
} // namespace Gvc

#endif

