#pragma once
#include "pch.h"

namespace ntt
{
    enum class NodeType
    {
        PROGRAM,
        EXPRESSION,
        INDEX,

        ATOMIC,

        INVALID,
        COUNT,
    };
} // namespace ntt
