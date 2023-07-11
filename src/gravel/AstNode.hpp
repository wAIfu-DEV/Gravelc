#ifndef _GVC_H_ASTNODE_
#define _GVC_H_ASTNODE_

#include <vector>

namespace Gvc {
namespace Ast {

enum AST_TYPE {
    NONE,
    ROOT,
    NODE,
    STRING,
    NUMBER,
    HEX_NUMBER,
    FUNC_CALL,
    FUNC_DEF,
    TYPE,
    LABEL,
    STACK_VAR,
    HEAP_VAR,
    RODATA_VAR,
    BODY,
    ARGUMENTS,
    EXPRESSION
};

struct AstNode {
    AST_TYPE type = AST_TYPE::NONE;
    int16_t depth = 0;
    uint16_t nth_child = 0;
    AstNode* parent = nullptr;
    std::vector<AstNode> children = {};  
};

} // namespace Ast
} // namespace Gvc


#endif // _GVC_H_ASTNODE_