#pragma once
#include <string>
#include <vector>
#include "token.h"

namespace ntt
{
    /**
     * Simple interface for converting the input string into predefined tokens.
     */
    class Tokenizer
    {
    public:
        Tokenizer(const char *input);
        Tokenizer(const std::string &input);
        ~Tokenizer();

        /**
         * Get the tokens parsed from the input string.
         * @return A vector of tokens parsed from the input string.
         */
        inline const std::vector<Token> &GetTokens() const { return m_tokens; }

    private:
        /**
         * Actually perform the tokenization of the input string. This is called
         *      inside the constructor. The `m_tokens` will be populated after this call.
         */
        void TokenizeInput();

    private:
        std::vector<Token> m_tokens;
        std::string m_input;
    };
} // namespace ntt
