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

#define GETTER_SETTER_IMPL(type, member, tokenType) \
    template <>                                     \
    void Token::SetValue<type>(type value)          \
    {                                               \
        NTT_ASSERT(m_type == TokenType::tokenType); \
        m_value.member = value;                     \
    }                                               \
                                                    \
    template <>                                     \
    type Token::GetValue<type>() const              \
    {                                               \
        NTT_ASSERT(m_type == TokenType::tokenType); \
        return m_value.member;                      \
    }

    GETTER_SETTER_IMPL(u32, intValue, INTEGER);
} // namespace ntt
