#pragma once
#include "pch.h"

namespace ntt
{
    enum class ErrorType
    {
        REDUNDANT_DELIMITER,

        MISSING_VARIABLE_NAME,

        MISSING_CONDITION,
        MISSING_BLOCK,

        MISSING_SEMICOLON,

        MISSING_END_BRACKET,
        MISSING_LEFT_OPERAND,
        MISSING_RIGHT_OPERAND,

        NO_ERROR,
        COUNT,
    };

    String ErrorTypeToString(ErrorType error);
}