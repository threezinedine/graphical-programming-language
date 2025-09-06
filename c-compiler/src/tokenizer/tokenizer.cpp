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
                TokenType::OPEN_BRACKET,
                {
                    "^\\(",
                    "^\\{",
                    "^\\[",
                },
            },
            {
                TokenType::CLOSE_BRACKET,
                {
                    "^\\)",
                    "^\\}",
                    "^\\]",
                },
            },
            {
                TokenType::KEYWORD,
                {
                    // types
                    "^number",
                    "^string",
                    "^boolean",
                    "^null",

                    // control flow
                    "^if",
                    "^else",
                    "^while",
                    "^for",

                    // declaration
                    "^const",
                    "^let",
                    "^function",
                    "^class",
                },
            },
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
            {
                TokenType::STRING,
                {
                    "^\"((?:[^\"\\\\]|\\\\.)*)\"",
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

            b8 hasMatched = NTT_FALSE;
            for (const auto &[tokenType, regexList] : regexes)
            {
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
                        case TokenType::STRING:
                        case TokenType::KEYWORD:
                        case TokenType::OPEN_BRACKET:
                        case TokenType::CLOSE_BRACKET:
                        {
                            token.SetValue<std::string>(matchedStr);
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

            if (!hasMatched)
            {
                // find the text until next non ' ' and '\n'
                // or end of the file as invalid token.

                std::string temporaryInvalidToken = "";
                u32 characterIndex = 0;
                u32 temporaryContentLength = (u32)temporaryContent.length();

                while (characterIndex < temporaryContentLength)
                {
                    char currentCharacter = temporaryContent[characterIndex];

                    if (currentCharacter != ' ' && currentCharacter != '\n')
                    {
                        temporaryInvalidToken += currentCharacter;
                        characterIndex++;
                    }
                    else
                    {
                        Token newInvalidToken(TokenType::INVALID);
                        newInvalidToken.SetValue<std::string>(temporaryInvalidToken);
                        temporaryContent = temporaryContent.substr((u32)temporaryInvalidToken.length());
                        m_tokens.push_back(newInvalidToken);
                        break;
                    }
                }

                if (temporaryInvalidToken.length() > 0 && characterIndex == temporaryContentLength) // end of line
                {
                    Token newInvalidToken(TokenType::INVALID);
                    newInvalidToken.SetValue<std::string>(temporaryInvalidToken);
                    m_tokens.push_back(newInvalidToken);
                    temporaryContent = "";
                }
            }
        }
    }
} // namespace ntt
