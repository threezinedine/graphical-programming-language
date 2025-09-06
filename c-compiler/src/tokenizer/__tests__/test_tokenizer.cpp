#include "test_common.h"
#include "tokenizer/tokenizer.h"

using namespace ntt;

void AssertIntegerToken(const Token &token, u32 value)
{
    ASSERT_EQ(token.GetType(), TokenType::INTEGER);
    EXPECT_THAT(token.GetValue<u32>(), ::testing::Eq(value));
}

void AssertFloatToken(const Token &token, f32 value)
{
    ASSERT_EQ(token.GetType(), TokenType::FLOAT);
    EXPECT_THAT(token.GetValue<f32>(), ::testing::FloatEq(value));
}

void AssertInvalidToken(const Token &token, const std::string &value)
{
    ASSERT_EQ(token.GetType(), TokenType::INVALID);
    EXPECT_THAT(token.GetValue<std::string>(), ::testing::StrEq(value));
}

void AssertStringToken(const Token &token, const std::string &value)
{
    ASSERT_EQ(token.GetType(), TokenType::STRING);
    EXPECT_THAT(token.GetValue<std::string>(), ::testing::StrEq(value));
}

void AssertKeywordToken(const Token &token, const std::string &value)
{
    ASSERT_EQ(token.GetType(), TokenType::KEYWORD);
    EXPECT_THAT(token.GetValue<std::string>(), ::testing::StrEq(value));
}

void AssertIdentifierToken(const Token &token, const std::string &value)
{
    ASSERT_EQ(token.GetType(), TokenType::IDENTIFIER);
    EXPECT_THAT(token.GetValue<std::string>(), ::testing::StrEq(value));
}

void AssertBracketToken(const Token &token, const std::string &value)
{
    ASSERT_EQ(token.GetType(), TokenType::BRACKET);
    EXPECT_THAT(token.GetValue<std::string>(), ::testing::StrEq(value));
}

void AssertDelimiterToken(const Token &token, const std::string &value)
{
    ASSERT_EQ(token.GetType(), TokenType::DELIMITER);
    EXPECT_THAT(token.GetValue<std::string>(), ::testing::StrEq(value));
}

void AssertOperatorToken(const Token &token, const std::string &value)
{
    ASSERT_EQ(token.GetType(), TokenType::OPERATOR);
    EXPECT_THAT(token.GetValue<std::string>(), ::testing::StrEq(value));
}

#define INTEGER_TOKEN_TESTING(input, value)                  \
    {                                                        \
        Tokenizer tokenizer(input);                          \
        AssertIntegerToken(tokenizer.GetTokens()[0], value); \
    }

#define FLOAT_TOKEN_TESTING(input, value)                  \
    {                                                      \
        Tokenizer tokenizer(input);                        \
        AssertFloatToken(tokenizer.GetTokens()[0], value); \
    }

#define INVALID_TOKEN_TESTING(input, value)                  \
    {                                                        \
        Tokenizer tokenizer(input);                          \
        AssertInvalidToken(tokenizer.GetTokens()[0], value); \
    }

#define STRING_TOKEN_TESTING(input, value)                  \
    {                                                       \
        Tokenizer tokenizer(input);                         \
        AssertStringToken(tokenizer.GetTokens()[0], value); \
    }

#define KEYWORD_TOKEN_TESTING(input, value)                  \
    {                                                        \
        Tokenizer tokenizer(input);                          \
        AssertKeywordToken(tokenizer.GetTokens()[0], value); \
    }

#define BRACKET_TOKEN_TESTING(input, value)                  \
    {                                                        \
        Tokenizer tokenizer(input);                          \
        AssertBracketToken(tokenizer.GetTokens()[0], value); \
    }

#define DELIMITER_TOKEN_TESTING(input, value)                  \
    {                                                          \
        Tokenizer tokenizer(input);                            \
        AssertDelimiterToken(tokenizer.GetTokens()[0], value); \
    }

#define IDENTIFIER_TOKEN_TESTING(input, value)                  \
    {                                                           \
        Tokenizer tokenizer(input);                             \
        AssertIdentifierToken(tokenizer.GetTokens()[0], value); \
    }

#define OPERATOR_TOKEN_TESTING(input, value)                  \
    {                                                         \
        Tokenizer tokenizer(input);                           \
        AssertOperatorToken(tokenizer.GetTokens()[0], value); \
    }

TEST(TokenizerTest, IntegerToken)
{
    INTEGER_TOKEN_TESTING("12", 12);
    INTEGER_TOKEN_TESTING("0", 0);
}

TEST(TokenizerTest, FloatToken)
{
    FLOAT_TOKEN_TESTING("12.", 12.0f);
    FLOAT_TOKEN_TESTING(".12", 0.12f);
    FLOAT_TOKEN_TESTING("3.12", 3.12f);
    FLOAT_TOKEN_TESTING("23.12", 23.12f);
    FLOAT_TOKEN_TESTING("0.", 0.0f);
}

TEST(TokenizerTest, MultipleMixedIntegersAndFloats)
{
    Tokenizer tokenizer("12 0 12. \n .12 3.12 23.12 0.");
    const auto &tokens = tokenizer.GetTokens();
    ASSERT_EQ(tokens.size(), 7);

    AssertIntegerToken(tokens[0], 12);
    AssertIntegerToken(tokens[1], 0);
    AssertFloatToken(tokens[2], 12.0f);
    AssertFloatToken(tokens[3], 0.12f);
    AssertFloatToken(tokens[4], 3.12f);
    AssertFloatToken(tokens[5], 23.12f);
    AssertFloatToken(tokens[6], 0.0f);
}

TEST(TokenizerTest, TokenizeInvalidToken)
{
    INVALID_TOKEN_TESTING("`", "`");
}

TEST(TokenizerTest, MultipleMixedIntegersAndInvalidToken)
{
    Tokenizer tokenizer("12 0 \n ` 3.12 ");
    const auto &tokens = tokenizer.GetTokens();
    ASSERT_EQ(tokens.size(), 4);

    AssertIntegerToken(tokens[0], 12);
    AssertIntegerToken(tokens[1], 0);
    AssertInvalidToken(tokens[2], "`");
    AssertFloatToken(tokens[3], 3.12f);
}

TEST(TokenizerTest, TokenizeString)
{
    STRING_TOKEN_TESTING(R"("Hello World")", R"("Hello World")");
    STRING_TOKEN_TESTING(R"("Hello \" World")", R"("Hello \" World")");
}

TEST(TokenizerTest, MutipleMixedIntegersFloatsStringsAndInvalids)
{
    Tokenizer tokenizer(R"(
 0 "Testing \" World""Translate" 
` 3.12 )");
    const auto &tokens = tokenizer.GetTokens();
    ASSERT_EQ(tokens.size(), 5);

    AssertIntegerToken(tokens[0], 0);
    AssertStringToken(tokens[1], R"("Testing \" World")");
    AssertStringToken(tokens[2], R"("Translate")");
    AssertInvalidToken(tokens[3], "`");
    AssertFloatToken(tokens[4], 3.12f);
}

TEST(TokenizerTest, TokenizeKeywords)
{
    // types
    {
        KEYWORD_TOKEN_TESTING("number", "number");
        KEYWORD_TOKEN_TESTING("string", "string");
        KEYWORD_TOKEN_TESTING("boolean", "boolean");
        KEYWORD_TOKEN_TESTING("null", "null");
    }

    // control flow
    {
        KEYWORD_TOKEN_TESTING("if", "if");
        KEYWORD_TOKEN_TESTING("else", "else");
        KEYWORD_TOKEN_TESTING("while", "while");
        KEYWORD_TOKEN_TESTING("for", "for");
    }

    // declaration
    {
        KEYWORD_TOKEN_TESTING("const", "const");
        KEYWORD_TOKEN_TESTING("let", "let");
        KEYWORD_TOKEN_TESTING("function", "function");
        KEYWORD_TOKEN_TESTING("class", "class");
    }
}

TEST(TokenizerTest, TokenizeBrackets)
{
    BRACKET_TOKEN_TESTING("(", "(");
    BRACKET_TOKEN_TESTING(")", ")");
    BRACKET_TOKEN_TESTING("{", "{");
    BRACKET_TOKEN_TESTING("}", "}");
    BRACKET_TOKEN_TESTING("[", "[");
    BRACKET_TOKEN_TESTING("]", "]");
}

TEST(TokenizerTest, TokenizeDelimiter)
{
    DELIMITER_TOKEN_TESTING(",", ",");
    DELIMITER_TOKEN_TESTING(";", ";");
}

TEST(TokenizerTest, TokenizeIdentifier)
{
    IDENTIFIER_TOKEN_TESTING("myVar", "myVar");
    IDENTIFIER_TOKEN_TESTING("_hidden", "_hidden");
    IDENTIFIER_TOKEN_TESTING("abc123", "abc123");
}

TEST(TokenizerTest, InvalidIndentifier)
{
    INVALID_TOKEN_TESTING("123abc", "123abc");
    INVALID_TOKEN_TESTING("123abc423", "123abc423");
}

TEST(TokenizerTest, TokenizeOperators)
{
    OPERATOR_TOKEN_TESTING("+", "+");
    OPERATOR_TOKEN_TESTING("-", "-");
    OPERATOR_TOKEN_TESTING("*", "*");
    OPERATOR_TOKEN_TESTING("/", "/");
    OPERATOR_TOKEN_TESTING("%", "%");
    OPERATOR_TOKEN_TESTING("^", "^");
    OPERATOR_TOKEN_TESTING("@", "@");
    OPERATOR_TOKEN_TESTING("!", "!");
    OPERATOR_TOKEN_TESTING("|", "|");
    OPERATOR_TOKEN_TESTING("&", "&");

    OPERATOR_TOKEN_TESTING("=", "=");

    OPERATOR_TOKEN_TESTING("++", "++");
    OPERATOR_TOKEN_TESTING("--", "--");

    OPERATOR_TOKEN_TESTING("+=", "+=");
    OPERATOR_TOKEN_TESTING("-=", "-=");
    OPERATOR_TOKEN_TESTING("*=", "*=");
    OPERATOR_TOKEN_TESTING("/=", "/=");
    OPERATOR_TOKEN_TESTING("%=", "%=");
    OPERATOR_TOKEN_TESTING("&=", "&=");
    OPERATOR_TOKEN_TESTING("|=", "|=");
    OPERATOR_TOKEN_TESTING("^=", "^=");
    OPERATOR_TOKEN_TESTING("@=", "@=");

    OPERATOR_TOKEN_TESTING(">", ">");
    OPERATOR_TOKEN_TESTING("<", "<");
    OPERATOR_TOKEN_TESTING("==", "==");
    OPERATOR_TOKEN_TESTING("!=", "!=");
    OPERATOR_TOKEN_TESTING(">=", ">=");
    OPERATOR_TOKEN_TESTING("<=", "<=");

    OPERATOR_TOKEN_TESTING("&&", "&&");
    OPERATOR_TOKEN_TESTING("||", "||");
}

TEST(TokenizerTest, FinalMix)
{
    Tokenizer tokenizer(R"(
const pi = 3.14;
let radius = 10;
if (radius > 0) {
    let area = pi * radius * radius;
    print("Area is: " + area);
} else {
    print("Invalid radius");
}
)");

    const auto &tokens = tokenizer.GetTokens();
    u32 index = 0;

    AssertKeywordToken(tokens[index++], "const");
    AssertIdentifierToken(tokens[index++], "pi");
    AssertOperatorToken(tokens[index++], "=");
    AssertFloatToken(tokens[index++], 3.14f);
    AssertDelimiterToken(tokens[index++], ";");
    AssertKeywordToken(tokens[index++], "let");
    AssertIdentifierToken(tokens[index++], "radius");
    AssertOperatorToken(tokens[index++], "=");
    AssertIntegerToken(tokens[index++], 10);
    AssertDelimiterToken(tokens[index++], ";");
    AssertKeywordToken(tokens[index++], "if");
    AssertBracketToken(tokens[index++], "(");
    AssertIdentifierToken(tokens[index++], "radius");
    AssertOperatorToken(tokens[index++], ">");
    AssertIntegerToken(tokens[index++], 0);
    AssertBracketToken(tokens[index++], ")");
    AssertBracketToken(tokens[index++], "{");
    AssertKeywordToken(tokens[index++], "let");
    AssertIdentifierToken(tokens[index++], "area");
    AssertOperatorToken(tokens[index++], "=");
    AssertIdentifierToken(tokens[index++], "pi");
    AssertOperatorToken(tokens[index++], "*");
    AssertIdentifierToken(tokens[index++], "radius");
    AssertOperatorToken(tokens[index++], "*");
    AssertIdentifierToken(tokens[index++], "radius");
    AssertDelimiterToken(tokens[index++], ";");
    AssertIdentifierToken(tokens[index++], "print");
    AssertBracketToken(tokens[index++], "(");
    AssertStringToken(tokens[index++], R"("Area is: ")");
    AssertOperatorToken(tokens[index++], "+");
    AssertIdentifierToken(tokens[index++], "area");
    AssertBracketToken(tokens[index++], ")");
    AssertDelimiterToken(tokens[index++], ";");
    AssertBracketToken(tokens[index++], "}");
    AssertKeywordToken(tokens[index++], "else");
    AssertBracketToken(tokens[index++], "{");
    AssertIdentifierToken(tokens[index++], "print");
    AssertBracketToken(tokens[index++], "(");
    AssertStringToken(tokens[index++], R"("Invalid radius")");
    AssertBracketToken(tokens[index++], ")");
    AssertDelimiterToken(tokens[index++], ";");
    AssertBracketToken(tokens[index++], "}");
}