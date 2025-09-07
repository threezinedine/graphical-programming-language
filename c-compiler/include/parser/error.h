#pragma once
#include "pch.h"

namespace ntt
{
    enum class ErrorType
    {
        MISSING_END_BRACKET,
        MISSING_RIGHT_OPERAND,

        NO_ERROR,
        COUNT,
    };

    String ErrorTypeToString(ErrorType error);
}