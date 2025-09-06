#pragma once

namespace ntt
{
    enum class TokenType
    {
        FLOAT,
        INTEGER,
        STRING,
        KEYWORD,
        BRACKET,
        DELIMITER,

        INVALID,
        COUNT,
    };
} // namespace ntt
