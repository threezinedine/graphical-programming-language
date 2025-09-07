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

        INVALID,
        COUNT,
    };
} // namespace ntt
