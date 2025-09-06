#include "tokenizer/tokenizer.h"

namespace ntt
{
    Tokenizer::Tokenizer(const char *input) : m_input(input)
    {
        TokenizeInput();
    }

    Tokenizer::Tokenizer(const std::string &input) : m_input(input)
    {
        TokenizeInput();
    }

    Tokenizer::~Tokenizer()
    {
    }

    void Tokenizer::TokenizeInput()
    {
        Token token(TokenType::INTEGER);
        token.SetValue<u32>(12);
        m_tokens.push_back(token);
    }
} // namespace ntt
