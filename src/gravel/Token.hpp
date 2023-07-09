#ifndef _GVC_H_TOKEN_
#define _GVC_H_TOKEN_

#include <iostream>

namespace Gvc {

enum TOK_TYPE {
    NONE = 0,
    /* one char */
    POINTER = 1,
    COLON = 2,
    PARENTHESIS_OPEN = 3,
    PARENTHESIS_CLOSE = 4,
    BRACE_OPEN = 5,
    BRACE_CLOSE = 6,
    BRACKET_OPEN = 7,
    BRACKET_CLOSE = 8,
    DOT = 9,
    COMMA = 10,
    SEMI_COLON = 11,
    /* multiple char */
    STRING = 12,
    CHAR = 13,
    NUMBER = 14,
    HEX_NUMBER = 15,
    NAME = 16,
    /* reserved names */
    KEYW_READONLY = 17,
    KEYW_STACK = 18,
    KEYW_HEAP = 19,
    KEYW_LINK = 20,
    KEYW_SECTION = 21,
    KEYW_RET = 22,
};

struct Token {
    std::string content = "";
    enum TOK_TYPE type = TOK_TYPE::NONE;
    uint16_t line = 1;
    uint16_t col = 1;
};

} // namespace Gvc


#endif