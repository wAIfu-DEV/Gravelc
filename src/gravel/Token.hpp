#ifndef _GVC_H_TOKEN_
#define _GVC_H_TOKEN_

#include <iostream>

namespace Gvc {
namespace Token {

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
    STRING = 100,
    CHAR = 101,
    NUMBER = 102,
    HEX_NUMBER = 103,
    NAME = 104,
    /* reserved names */
    KEYW_READONLY = 200,
    KEYW_STACK = 201,
    KEYW_HEAP = 202,
    KEYW_LINK = 203,
    KEYW_SECTION = 204,
    KEYW_RET = 205,
};

struct Tok {
    std::string content = "";
    enum TOK_TYPE type = TOK_TYPE::NONE;
    uint16_t line = 1;
    uint16_t col = 1;
};

}
} // namespace Gvc


#endif