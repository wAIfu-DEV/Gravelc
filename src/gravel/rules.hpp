#ifndef _GVC_H_RULES_
#define _GVC_H_RULES_

#include <vector>

#include "Token.hpp"
#include "AstNode.hpp"

namespace Gvc {
namespace GravelRules {

typedef std::pair<const char*, Token::TOK_TYPE> TokenMatch;

constexpr
size_t TOK_LOOKUP_TABLE_SIZE = 17;

typedef Token::TOK_TYPE TT;

constexpr
TokenMatch tok_lookup_table[TOK_LOOKUP_TABLE_SIZE] {
    /* single char tokens */
    {"*", TT::POINTER},
    {":", TT::COLON},
    {"(", TT::PARENTHESIS_OPEN},
    {")", TT::PARENTHESIS_CLOSE},
    {"{", TT::BRACE_OPEN},
    {"}", TT::BRACE_CLOSE},
    {"[", TT::BRACKET_OPEN},
    {"]", TT::BRACKET_CLOSE},
    {".", TT::DOT},
    {",", TT::COMMA},
    {";", TT::SEMI_COLON},
    /* reserved word */
    {"rodata", TT::KEYW_READONLY},
    {"stack", TT::KEYW_STACK},
    {"heap", TT::KEYW_HEAP},
    {"link", TT::KEYW_LINK},
    {"section", TT::KEYW_SECTION},
    {"ret", TT::KEYW_RET}
};

typedef Ast::AST_TYPE AT;

struct AstPatternNode {
    AT type = AT::NONE;
    AT dest = AT::NONE;
    bool discard = false;
};

struct PatternEntry {
    AT type;
    AstPatternNode patterns[16];
};

constexpr
size_t AST_PATTERNS_SIZE = 3;

constexpr
PatternEntry ast_patterns[AST_PATTERNS_SIZE] {
    {AT::EXPRESSION, {{AT::STRING, AT::BODY, false}}},
    {AT::EXPRESSION, {{AT::NUMBER, AT::BODY, false}}},
    {AT::EXPRESSION, {{AT::HEX_NUMBER, AT::BODY, false}}},
};

} // namespace GravelRules
} // namespace Gvc


#endif // _GVC_H_RULES_