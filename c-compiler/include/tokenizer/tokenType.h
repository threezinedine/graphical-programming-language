#pragma once
#include "pch.h"

namespace ntt
{
    enum class TokenType
    {
        FLOAT,
        INTEGER,
        BOOLEAN,
        STRING,
        KEYWORD,
        IDENTIFIER,
        BRACKET,
        DELIMITER,
        OPERATOR,
        TYPE_HINT,

        INVALID,
        COUNT,
    };

    String TokenTypeToString(TokenType type);
} // namespace ntt
