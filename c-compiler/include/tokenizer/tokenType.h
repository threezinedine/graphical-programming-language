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
        OPERATOR,

        INVALID,
        COUNT,
    };
} // namespace ntt
