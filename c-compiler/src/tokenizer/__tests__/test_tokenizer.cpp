#include "test_common.h"
#include "tokenizer/tokenizer.h"

using namespace ntt;

void AssertIntegerToken(const Token &token, u32 value)
{
    ASSERT_EQ(token.GetType(), TokenType::INTEGER);
    ASSERT_EQ(token.GetValue<u32>(), value);
}

TEST(TokenizerTest, IntegerToken)
{
    Tokenizer tokenizer("12");
    AssertIntegerToken(tokenizer.GetTokens()[0], 12);
}