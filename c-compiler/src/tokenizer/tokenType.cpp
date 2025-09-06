#include "tokenizer/tokenType.h"

namespace ntt
{
    String TokenTypeToString(TokenType type)
    {
        switch (type)
        {
        case TokenType::FLOAT:
            return "FLOAT";
        case TokenType::INTEGER:
            return "INTEGER";
        case TokenType::STRING:
            return "STRING";
        case TokenType::KEYWORD:
            return "KEYWORD";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::BRACKET:
            return "BRACKET";
        case TokenType::DELIMITER:
            return "DELIMITER";
        case TokenType::OPERATOR:
            return "OPERATOR";
        case TokenType::INVALID:
            return "INVALID";
        case TokenType::COUNT:
            return "COUNT";
        default:
            return "UNKNOWN";
        }
    }
} // namespace ntt
