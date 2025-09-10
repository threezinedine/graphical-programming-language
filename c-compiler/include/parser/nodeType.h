#pragma once
#include "pch.h"

namespace ntt
{
    enum class NodeType
    {
        PROGRAM,
        EXPRESSION,
        INDEX,
        BLOCK,
        FUNCTION_CALL,
        CALL_ARGUMENTS,

        IF_STATEMENT,
        STATEMENT,
        ATOMIC,

        UNARY_OPERATION,
        OPERATION,

        INVALID,
        COUNT,
    };

    String NodeTypeToString(NodeType type);
} // namespace ntt
