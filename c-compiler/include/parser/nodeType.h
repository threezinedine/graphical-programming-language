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

        STATEMENT,
        ATOMIC,

        UNARY_OPERATION,
        OPERATION,

        INVALID,
        COUNT,
    };
} // namespace ntt
