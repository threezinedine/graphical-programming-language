#include "parser/nodeType.h"

namespace ntt
{
    String NodeTypeToString(NodeType type)
    {
        switch (type)
        {
        case NodeType::PROGRAM:
            return "PROGRAM";
        case NodeType::EXPRESSION:
            return "EXPRESSION";
        case NodeType::INDEX:
            return "INDEX";
        case NodeType::BLOCK:
            return "BLOCK";
        case NodeType::FUNCTION_CALL:
            return "FUNCTION_CALL";
        case NodeType::CALL_ARGUMENTS:
            return "CALL_ARGUMENTS";
        case NodeType::IF_STATEMENT:
            return "IF_STATEMENT";
        case NodeType::STATEMENT:
            return "STATEMENT";
        case NodeType::ATOMIC:
            return "ATOMIC";
        case NodeType::UNARY_OPERATION:
            return "UNARY_OPERATION";
        case NodeType::OPERATION:
            return "OPERATION";
        case NodeType::INVALID:
            return "INVALID";
        case NodeType::COUNT:
            return "COUNT";
        default:
            return "UNKNOWN";
        }
    }
} // namespace ntt
