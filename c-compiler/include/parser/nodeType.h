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

        ATOMIC,

        UNARY_OPERATION,

        INVALID,
        COUNT,
    };
} // namespace ntt
