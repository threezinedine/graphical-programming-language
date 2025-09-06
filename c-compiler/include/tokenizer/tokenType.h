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
        DELIMITER,

        INVALID,
        COUNT,
    };
} // namespace ntt
