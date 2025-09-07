#include "tokenizer/tokenizer.h"
#include <utility>
#include <regex>
#include <set>

namespace ntt
{
    namespace
    {
        std::set<std::string> keywords = {
            // types
            "number",
            "string",
            "boolean",
            "null",

            // control flow
            "if",
            "else",
            "while",
            "for",

            // declaration
            "const",
            "let",
            "function",
            "class",
        };

        typedef std::pair<TokenType, std::vector<std::string>> TokenRegexPair;

        std::vector<TokenRegexPair> regexes = {
            {
                TokenType::OPERATOR,
                {
                    "^\\+=",
                    "^\\-=",
                    "^\\*=",
                    "^\\/=",
                    "^\\%=",
                    "^\\&=",
                    "^\\|=",
                    "^\\^=",
                    "^\\@=",

                    "^==",
                    "^!=",
                    "^>=",
                    "^<=",
                    "^>",
                    "^<",

                    "^&&",
                    "^\\|\\|",

                    "^\\++",
                    "^\\--",

                    "^\\+",
                    "^\\-",
                    "^\\*",
                    "^\\/",
                    "^\\%",
                    "^\\^",
                    "^@",
                    "^!",
                    "^\\|",
                    "^&",

                    "^=",
                },
            },
            {
                TokenType::INVALID,
                {
                    "^[0-9]+[a-zA-Z][0-9a-zA-Z]*",
                },
            },
            {
                TokenType::BOOLEAN,
                {
                    "^true",
                    "^false",
                },
            },
            {
                TokenType::IDENTIFIER,
                {
                    "^[a-zA-Z_][a-zA-Z0-9_]*",
                },
            },
            {
                TokenType::DELIMITER,
                {
                    "^\\;",
                    "^\\,",
                },
            },
            {
                TokenType::BRACKET,
                {
                    "^\\(",
                    "^\\{",
                    "^\\[",
                    "^\\)",
                    "^\\}",
                    "^\\]",
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
        PreProcessContent();
        TokenizeInput();
    }

    Tokenizer::Tokenizer(const std::string &input) : m_input(input)
    {
        PreProcessContent();
        TokenizeInput();
    }

    Tokenizer::~Tokenizer()
    {
    }

    void Tokenizer::PreProcessContent()
    {
        std::string temporaryContent = "";
        u32 characterIndex = 0;
        u32 inputLength = (u32)m_input.length();

        while (characterIndex < inputLength)
        {
            char curentCharacter = m_input[characterIndex];

            if (curentCharacter != '\n')
            {
                temporaryContent += curentCharacter;
            }
            else
            {
                temporaryContent += ' ';
            }

            characterIndex++;
        }

        NTT_ASSERT(temporaryContent.length() == m_input.length());
        m_input = std::move(temporaryContent);
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
            u32 currentRawInputIndex = (u32)m_input.length() - (u32)temporaryContent.length();
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
                        Token token(tokenType, currentRawInputIndex);

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
                        case TokenType::IDENTIFIER:
                        {
                            if (keywords.find(matchedStr) != keywords.end())
                            {
                                token = Token(TokenType::KEYWORD, currentRawInputIndex);
                            }
                            else
                            {
                                token = Token(TokenType::IDENTIFIER, currentRawInputIndex);
                            }
                            token.SetValue<std::string>(matchedStr);
                            break;
                        }
                        case TokenType::BOOLEAN:
                        {
                            b8 boolValue = (matchedStr == "true");
                            token.SetValue<b8>(boolValue);
                            token.SetLength((u32)matchedStr.length());
                            break;
                        }
                        case TokenType::STRING:
                        case TokenType::DELIMITER:
                        case TokenType::BRACKET:
                        case TokenType::INVALID:
                        case TokenType::OPERATOR:
                        {
                            token.SetValue<std::string>(matchedStr);
                            break;
                        }
                        default:
                            break;
                        }

                        token.SetLength((u32)matchedStr.length());
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
                // find the text until next non ' '
                // or end of the file as invalid token.

                std::string temporaryInvalidToken = "";
                u32 characterIndex = 0;
                u32 temporaryContentLength = (u32)temporaryContent.length();

                while (characterIndex < temporaryContentLength)
                {
                    char currentCharacter = temporaryContent[characterIndex];

                    if (currentCharacter != ' ')
                    {
                        temporaryInvalidToken += currentCharacter;
                        characterIndex++;
                    }
                    else
                    {
                        Token newInvalidToken(TokenType::INVALID, currentRawInputIndex);
                        newInvalidToken.SetValue<std::string>(temporaryInvalidToken);
                        newInvalidToken.SetLength((u32)temporaryInvalidToken.length());
                        temporaryContent = temporaryContent.substr((u32)temporaryInvalidToken.length());
                        m_tokens.push_back(newInvalidToken);
                        break;
                    }
                }

                if (temporaryInvalidToken.length() > 0 && characterIndex == temporaryContentLength) // end of line
                {
                    Token newInvalidToken(TokenType::INVALID, currentRawInputIndex);
                    newInvalidToken.SetValue<std::string>(temporaryInvalidToken);
                    newInvalidToken.SetLength((u32)temporaryInvalidToken.length());
                    m_tokens.push_back(newInvalidToken);
                    temporaryContent = "";
                }
            }
        }
    }
} // namespace ntt
