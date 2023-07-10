#ifndef _GVC_H_LEXER_
#define _GVC_H_LEXER_

#include "../main.cpp"
#include "../IO.hpp"
#include "../files.hpp"
#include "Token.hpp"
#include "rules.hpp"

namespace Gvc {
namespace GravelLexer {

enum LEX_MODE {
    DEFAULT,
    STRING,
    CHAR,
    NUMBER,
    COMMENT,
};

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
    std::vector<Token> tokens = {};
    std::vector<char> tok_buff = {};
}

bool isNotEOF() {
    return LexerStatus::char_index + 1 < LexerStatus::data->length();
}

char peakNext() {
    if (LexerStatus::char_index >= LexerStatus::data->length()) return '\0';
    return LexerStatus::data->at(LexerStatus::char_index);
}

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

void pushToken(enum TOK_TYPE type) {
    std::string s = std::string(
        LexerStatus::tok_buff.begin(), LexerStatus::tok_buff.end());

    if (s == "") return;

    Token t = Token();
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

TOK_TYPE matchC(char c) {
    // Check if is single char token or resered keyword
    for (size_t i = 0; i < GravelRules::tok_lookup_table.size(); ++i) {
        if (std::string(1,c) == GravelRules::tok_lookup_table[i].first) {
            return GravelRules::tok_lookup_table[i].second;
        }
    }
    return TOK_TYPE::NONE;
}

TOK_TYPE matchToken() {
    std::string tok = std::string(
        LexerStatus::tok_buff.begin(), LexerStatus::tok_buff.end());

    // Check if is single char token or resered keyword
    for (size_t i = 0; i < GravelRules::tok_lookup_table.size(); ++i) {
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

void switchOnComment() {
    if (LexerStatus::lexing_mode == LEX_MODE::STRING) {}
    else if (LexerStatus::lexing_mode == LEX_MODE::CHAR) {}
    else {
        pushToken(matchToken());
        LexerStatus::lexing_mode = LEX_MODE::COMMENT;
    }
}

void switchOnCharQuote() {
    if (LexerStatus::lexing_mode == LEX_MODE::CHAR) {
        pushToken(TOK_TYPE::CHAR);
        LexerStatus::lexing_mode = LEX_MODE::DEFAULT;
    }
    else
    if (LexerStatus::lexing_mode == LEX_MODE::STRING) {}
    else {
        pushToken(matchToken());
        LexerStatus::lexing_mode = LEX_MODE::CHAR;
    }
}

void switchOnStringQuote() {
    if (LexerStatus::lexing_mode == LEX_MODE::STRING) {
        pushToken(TOK_TYPE::STRING);
        LexerStatus::lexing_mode = LEX_MODE::DEFAULT;
    }
    else
    if (LexerStatus::lexing_mode == LEX_MODE::CHAR) {}
    else {
        pushToken(matchToken());
        LexerStatus::lexing_mode = LEX_MODE::STRING;
    }  
}

bool isComment(char c) {
    if (LexerStatus::escape_next) return false;
    return (c == '/' && peakNext() == '/');
}

bool isCharQuote(char c) {
    if (LexerStatus::escape_next) return false;
    return (c == '`');
}

bool isStringQuote(char c) {
    if (LexerStatus::escape_next) return false;
    return (c == '"' || c == '\'');
}

void _verifyTokens() {

    IO::log("verifying tokens . . .\n");

    if (LexerStatus::tokens.size() == 0) {
        IO::log("input file was empty\n");
    }

    for (size_t i = 0; i < LexerStatus::tokens.size(); ++i) {
        Token t = LexerStatus::tokens.at(i);
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

void _lex(const std::string& file_path, std::string* content_ptr) {
    IO::log(s("lexing ") + file_path + "\n");

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

        TOK_TYPE char_tok_type = matchC(c);
        if (char_tok_type != TOK_TYPE::NONE) {
            pushToken(matchToken());
            LexerStatus::tok_buff.push_back(c);
            pushToken(char_tok_type);
            continue;
        }

        if (isWhiteSpace(c)) {
            pushToken(matchToken());
            continue;
        }
        LexerStatus::tok_buff.push_back(c);
    }
}

void startLexing(const std::string& path) {
    std::string* file_content = Files::getContent(path);
    _lex(path, file_content);
    _verifyTokens();
    Files::tokens_map.push_back({path, LexerStatus::tokens});
    if (LexerStatus::has_errored) {
        IO::errCritical("There was an error during the lexing process.\n");
    }
}

} // namespace GravelLexer
} // namespace Gvc

#endif

