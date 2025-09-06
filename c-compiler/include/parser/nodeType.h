#pragma once
#include "pch.h"

namespace ntt
{
    enum class NodeType
    {
        PROGRAM,
        EXPRESSION,

        ATOMIC,

        INVALID,
        COUNT,
    };
} // namespace ntt
