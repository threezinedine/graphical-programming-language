#pragma once
#include "pch.h"

namespace ntt
{
    class Node;

    enum class NodeType
    {
        PROGRAM,
        EXPRESSION,
        INDEX,
        BLOCK,
        FUNCTION_CALL,
        CALL_ARGUMENTS,

        ATOMIC_EXPRESSION,

        IF_STATEMENT,
        VARIABLE_DEFINITION_STATEMENT,
        STATEMENT,
        ATOMIC,

        UNARY_OPERATION,
        OPERATION,

        INVALID,
        COUNT,
    };

    String NodeTypeToString(NodeType type);
} // namespace ntt
