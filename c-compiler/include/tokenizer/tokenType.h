#pragma once

namespace ntt
{
    enum class TokenType
    {
        FLOAT,
        INTEGER,
        STRING,
        KEYWORD,
        IDENTIFIER,
        BRACKET,
        DELIMITER,

        INVALID,
        COUNT,
    };
} // namespace ntt
