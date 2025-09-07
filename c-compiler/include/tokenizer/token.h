#pragma once
#include "tokenType.h"
#include "pch.h"

namespace ntt
{
    /**
     * All possible value whwich the token can hold type will be
     *      defined here.
     */
    struct TokenValue
    {
        union Number
        {
            u32 intValue;
            f32 floatValue;
        };

        Number numberValue;
        std::string stringValue;
    };

    class Token
    {
    public:
        Token(TokenType type, u32 startIndex);
        Token(const Token &other);
        ~Token();

        /**
         * @return The type of the token.
         */
        inline TokenType GetType() const { return m_type; }

        inline u32 GetStartIndex() const { return m_startIndex; }

        inline u32 GetLength() const { return m_length; }
        inline void SetLength(u32 length) { m_length = length; }

        template <typename T>
        void SetValue(T value);

        template <typename T>
        T GetValue() const;

        JSON ToJSON() const;

    private:
        TokenType m_type;
        TokenValue m_value;
        u32 m_startIndex;
        u32 m_length;
    };
} // namespace ntt
