#include "tokenizer/token.h"

namespace ntt
{
    Token::Token(TokenType type)
        : m_type(type)
    {
    }

    Token::Token(const Token &other)
        : m_type(other.m_type), m_value(other.m_value)
    {
    }

    Token::~Token()
    {
    }

#define ASSERT_TYPE_IN_ARRAY(...)                                 \
    {                                                             \
        std::vector<TokenType> types{__VA_ARGS__};                \
        b8 hasMatched = NTT_FALSE;                                \
        for (const auto &tokenType : types)                       \
        {                                                         \
            if (m_type == tokenType)                              \
            {                                                     \
                hasMatched = NTT_TRUE;                            \
                break;                                            \
            }                                                     \
        }                                                         \
        NTT_ASSERT_MSG(hasMatched, "Token type does not match."); \
    }

#define GETTER_SETTER_IMPL(type, member, ...) \
    template <>                               \
    void Token::SetValue<type>(type value)    \
    {                                         \
        ASSERT_TYPE_IN_ARRAY(__VA_ARGS__);    \
        m_value.member = value;               \
    }                                         \
                                              \
    template <>                               \
    type Token::GetValue<type>() const        \
    {                                         \
        ASSERT_TYPE_IN_ARRAY(__VA_ARGS__);    \
        return m_value.member;                \
    }

    GETTER_SETTER_IMPL(u32, numberValue.intValue, TokenType::INTEGER);
    GETTER_SETTER_IMPL(f32, numberValue.floatValue, TokenType::FLOAT);
    GETTER_SETTER_IMPL(std::string, stringValue, TokenType::STRING,
                       TokenType::INVALID, TokenType::KEYWORD,
                       TokenType::BRACKET,
                       TokenType::DELIMITER,
                       TokenType::IDENTIFIER,
                       TokenType::OPERATOR);

    JSON Token::ToJSON() const
    {
        JSON json;
        json["type"] = TokenTypeToString(m_type);
        switch (m_type)
        {
        case TokenType::INTEGER:
            json["value"] = m_value.numberValue.intValue;
            break;
        case TokenType::FLOAT:
            json["value"] = m_value.numberValue.floatValue;
            break;
        case TokenType::STRING:
        case TokenType::INVALID:
        case TokenType::KEYWORD:
        case TokenType::BRACKET:
        case TokenType::DELIMITER:
        case TokenType::IDENTIFIER:
        case TokenType::OPERATOR:
            json["value"] = m_value.stringValue;
            break;
        default:
            break;
        }
        return json;
    }
} // namespace ntt
