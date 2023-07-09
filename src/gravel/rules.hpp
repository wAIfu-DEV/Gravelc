#ifndef _GVC_H_RULES_
#define _GVC_H_RULES_

#include "Token.hpp"

namespace Gvc {
namespace GravelRules
{

typedef std::vector<std::pair<std::string, enum TOK_TYPE>> TokenMatchLookup;

TokenMatchLookup tok_lookup_table = {
    /* single char tokens */
    {"*", TOK_TYPE::POINTER},
    {":", TOK_TYPE::COLON},
    {"(", TOK_TYPE::PARENTHESIS_OPEN},
    {")", TOK_TYPE::PARENTHESIS_CLOSE},
    {"{", TOK_TYPE::BRACE_OPEN},
    {"}", TOK_TYPE::BRACE_CLOSE},
    {"[", TOK_TYPE::BRACKET_OPEN},
    {"]", TOK_TYPE::BRACKET_CLOSE},
    {".", TOK_TYPE::DOT},
    {",", TOK_TYPE::COMMA},
    {";", TOK_TYPE::SEMI_COLON},
    /* reserved word */
    {"rodata", TOK_TYPE::KEYW_READONLY},
    {"stack", TOK_TYPE::KEYW_STACK},
    {"heap", TOK_TYPE::KEYW_HEAP},
    {"link", TOK_TYPE::KEYW_LINK},
    {"section", TOK_TYPE::KEYW_SECTION},
    {"ret", TOK_TYPE::KEYW_RET}
};

} // namespace GravelRules
} // namespace Gvc


#endif // _GVC_H_RULES_