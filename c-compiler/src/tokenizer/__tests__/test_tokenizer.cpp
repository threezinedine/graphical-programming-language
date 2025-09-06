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

TEST(TokenizerTest, IntegerToken)
{
    INTEGER_TOKEN_TESTING("12", 12);
    INTEGER_TOKEN_TESTING("0", 0);
}

TEST(TokenizerTest, FloatToken)
{
    FLOAT_TOKEN_TESTING("12.", 12.0f);
}