#include "tokenizer/tokenizer.h"
#include <utility>
#include <regex>

namespace ntt
{
    namespace
    {
        typedef std::pair<TokenType, std::vector<std::string>> TokenRegexPair;

        std::vector<TokenRegexPair> regexes = {
            {
                TokenType::FLOAT,
                {
                    "^[0-9]*\\.[0-9]+",
                    "^[0-9]+\\.",
                    "^\\.[0-9]+",
                },
            },
            {
                TokenType::INTEGER,
                {
                    "^[0-9]+",
                },
            },
        };
    } // namespace anonymous

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
        std::string temporaryContent = m_input;

        while (temporaryContent.length() > 0)
        {
            // remove spaces and new line characters
            {
                while (true)
                {
                    bool isTemporaryContentEmpty = temporaryContent.length() == 0;

                    if (isTemporaryContentEmpty)
                    {
                        break;
                    }

                    char firstTemporaryCharacter = temporaryContent[0];

                    if (firstTemporaryCharacter == ' ' || firstTemporaryCharacter == '\n')
                    {
                        temporaryContent = temporaryContent.substr(1);
                    }
                    else
                    {
                        break;
                    }
                }

                if (temporaryContent.length() == 0)
                {
                    break;
                }
            }

            for (const auto &[tokenType, regexList] : regexes)
            {
                b8 hasMatched = NTT_FALSE;
                for (const auto &regex : regexList)
                {
                    std::smatch match;
                    std::regex rgx(regex);
                    if (std::regex_search(temporaryContent, match, rgx))
                    {
                        std::string matchedStr = match.str(0);
                        temporaryContent = temporaryContent.substr(matchedStr.length());
                        Token token(tokenType);

                        switch (tokenType)
                        {
                        case TokenType::INTEGER:
                        {
                            u32 intValue = std::stoul(matchedStr);
                            token.SetValue<u32>(intValue);
                            break;
                        }
                        case TokenType::FLOAT:
                        {
                            f32 floatValue = std::stof(matchedStr);
                            token.SetValue<f32>(floatValue);
                            break;
                        }
                        default:
                            break;
                        }

                        hasMatched = NTT_TRUE;
                        m_tokens.push_back(token);
                        break;
                    }
                }

                if (hasMatched)
                {
                    break;
                }
            }
        }
    }
} // namespace ntt
