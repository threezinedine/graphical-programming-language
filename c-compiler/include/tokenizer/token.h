#pragma once
#include "tokenType.h"
#include "pch.h"

namespace ntt
{
    /**
     * All possible value whwich the token can hold type will be
     *      defined here.
     */
    union TokenValue
    {
        u32 intValue;
        f32 floatValue;
    };

    class Token
    {
    public:
        Token(TokenType type);
        Token(const Token &other);
        ~Token();

        /**
         * @return The type of the token.
         */
        inline TokenType GetType() const { return m_type; }

        template <typename T>
        void SetValue(T value);

        template <typename T>
        T GetValue() const;

    private:
        TokenType m_type;
        TokenValue m_value;
    };
} // namespace ntt
