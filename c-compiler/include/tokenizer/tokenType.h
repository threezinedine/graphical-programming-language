#pragma once

namespace ntt
{
    enum class TokenType
    {
        FLOAT,
        INTEGER,
        STRING,
        KEYWORD,
        OPEN_BRACKET,
        CLOSE_BRACKET,

        INVALID,
        COUNT,
    };
} // namespace ntt
