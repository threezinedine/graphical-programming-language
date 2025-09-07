#include "test_common.h"
#include "tokenizer/tokenizer.h"

using namespace ntt;

#define LINE_PROPAGATION(call, ...) call(__VA_ARGS__, __LINE__)

#define FUNC_ASSERT_TOKEN_IMPL(tokenType, valueType, eqCallback, valueToStringSuffix)                              \
    void Assert##tokenType##Token(const Token &token, valueType value, i32 startIndex, u32 length, u32 line)       \
    {                                                                                                              \
        char errorBuffer[256];                                                                                     \
        sprintf_s(errorBuffer, " at line %d (token: %s) does not match.\n", line, token.ToJSON().dump(4).c_str()); \
        ASSERT_EQ(token.GetType(), TokenType::tokenType)                                                           \
            << "Assert type = " << #tokenType << std::endl                                                         \
            << errorBuffer;                                                                                        \
        EXPECT_THAT(token.GetValue<valueType>(), ::testing::eqCallback(value))                                     \
            << "Value = " << token.GetValue<valueType>() valueToStringSuffix << std::endl                          \
            << errorBuffer;                                                                                        \
        if (startIndex != -1)                                                                                      \
        {                                                                                                          \
            EXPECT_THAT(token.GetStartIndex(), ::testing::Eq(u32(startIndex)))                                     \
                << "Start Index = " << startIndex << std::endl                                                     \
                << errorBuffer;                                                                                    \
        }                                                                                                          \
        EXPECT_THAT(token.GetLength(), ::testing::Eq(length))                                                      \
            << "Assert length = " << length << std::endl                                                           \
            << errorBuffer;                                                                                        \
    }

FUNC_ASSERT_TOKEN_IMPL(INTEGER, u32, Eq, );
FUNC_ASSERT_TOKEN_IMPL(FLOAT, f32, FloatEq, );
FUNC_ASSERT_TOKEN_IMPL(INVALID, std::string, StrEq, .c_str());
FUNC_ASSERT_TOKEN_IMPL(STRING, std::string, StrEq, .c_str());
FUNC_ASSERT_TOKEN_IMPL(KEYWORD, std::string, StrEq, .c_str());
FUNC_ASSERT_TOKEN_IMPL(IDENTIFIER, std::string, StrEq, .c_str());
FUNC_ASSERT_TOKEN_IMPL(BRACKET, std::string, StrEq, .c_str());
FUNC_ASSERT_TOKEN_IMPL(DELIMITER, std::string, StrEq, .c_str());
FUNC_ASSERT_TOKEN_IMPL(OPERATOR, std::string, StrEq, .c_str());

#define TOKEN_TESTING(tokenType, input, value, startIndex, length) \
    _TOKEN_TESTING(tokenType, input, value, startIndex, length, __LINE__)

#define _TOKEN_TESTING(tokenType, input, value, startIndex, length, line)                    \
    {                                                                                        \
        Tokenizer tokenizer(input);                                                          \
        Assert##tokenType##Token(tokenizer.GetTokens()[0], value, startIndex, length, line); \
    }

TEST(TokenizerTest, IntegerToken)
{
    TOKEN_TESTING(INTEGER, "12", 12, 0, 2);
    TOKEN_TESTING(INTEGER, "0", 0, 0, 1);
}

TEST(TokenizerTest, FloatToken)
{
    TOKEN_TESTING(FLOAT, "12.", 12.0f, 0, 3);
    TOKEN_TESTING(FLOAT, ".12", 0.12f, 0, 3);
    TOKEN_TESTING(FLOAT, "3.12", 3.12f, 0, 4);
    TOKEN_TESTING(FLOAT, "23.12", 23.12f, 0, 5);
    TOKEN_TESTING(FLOAT, "0.", 0.0f, 0, 2);
}

TEST(TokenizerTest, MultipleMixedIntegersAndFloats)
{
    Tokenizer tokenizer("12 0 12. \n .12 3.12 23.12 0.");
    const auto &tokens = tokenizer.GetTokens();
    ASSERT_EQ(tokens.size(), 7);

    LINE_PROPAGATION(AssertINTEGERToken, tokens[0], 12, 0, 2);
    LINE_PROPAGATION(AssertINTEGERToken, tokens[1], 0, 3, 1);
    LINE_PROPAGATION(AssertFLOATToken, tokens[2], 12.0f, 5, 3);
    LINE_PROPAGATION(AssertFLOATToken, tokens[3], 0.12f, 11, 3);
    LINE_PROPAGATION(AssertFLOATToken, tokens[4], 3.12f, 15, 4);
    LINE_PROPAGATION(AssertFLOATToken, tokens[5], 23.12f, 20, 5);
    LINE_PROPAGATION(AssertFLOATToken, tokens[6], 0.0f, 26, 2);
}

TEST(TokenizerTest, TokenizeInvalidToken)
{
    TOKEN_TESTING(INVALID, "`", "`", 0, 1);
}

TEST(TokenizerTest, MultipleMixedIntegersAndInvalidToken)
{
    Tokenizer tokenizer("12 0 \n ` 3.12 ");
    const auto &tokens = tokenizer.GetTokens();
    ASSERT_EQ(tokens.size(), 4);

    LINE_PROPAGATION(AssertINTEGERToken, tokens[0], 12, 0, 2);
    LINE_PROPAGATION(AssertINTEGERToken, tokens[1], 0, 3, 1);
    LINE_PROPAGATION(AssertINVALIDToken, tokens[2], "`", 7, 1);
    LINE_PROPAGATION(AssertFLOATToken, tokens[3], 3.12f, 9, 4);
}

TEST(TokenizerTest, TokenizeString)
{
    TOKEN_TESTING(STRING, R"("Hello World")", R"("Hello World")", 0, 13);
    TOKEN_TESTING(STRING, R"("Hello \" World")", R"("Hello \" World")", 0, 16);
}

TEST(TokenizerTest, MutipleMixedIntegersFloatsStringsAndInvalids)
{
    Tokenizer tokenizer(R"(
 0 "Testing \" World""Translate" 
` 3.12 )");
    const auto &tokens = tokenizer.GetTokens();
    ASSERT_EQ(tokens.size(), 5);

    LINE_PROPAGATION(AssertINTEGERToken, tokens[0], 0, 2, 1);
    LINE_PROPAGATION(AssertSTRINGToken, tokens[1], R"("Testing \" World")", 4, 18);
    LINE_PROPAGATION(AssertSTRINGToken, tokens[2], R"("Translate")", 22, 11);
    LINE_PROPAGATION(AssertINVALIDToken, tokens[3], "`", 35, 1);
    LINE_PROPAGATION(AssertFLOATToken, tokens[4], 3.12f, 37, 4);
}

TEST(TokenizerTest, TokenizeKeywords)
{
    // types
    {
        TOKEN_TESTING(KEYWORD, "number", "number", 0, 6);
        TOKEN_TESTING(KEYWORD, "string", "string", 0, 6);
        TOKEN_TESTING(KEYWORD, "boolean", "boolean", 0, 7);
        TOKEN_TESTING(KEYWORD, "null", "null", 0, 4);
    }

    // control flow
    {
        TOKEN_TESTING(KEYWORD, "if", "if", 0, 2);
        TOKEN_TESTING(KEYWORD, "else", "else", 0, 4);
        TOKEN_TESTING(KEYWORD, "while", "while", 0, 5);
        TOKEN_TESTING(KEYWORD, "for", "for", 0, 3);
    }

    // declaration
    {
        TOKEN_TESTING(KEYWORD, "const", "const", 0, 5);
        TOKEN_TESTING(KEYWORD, "let", "let", 0, 3);
        TOKEN_TESTING(KEYWORD, "function", "function", 0, 8);
        TOKEN_TESTING(KEYWORD, "class", "class", 0, 5);
    }
}

TEST(TokenizerTest, TokenizeBrackets)
{
    TOKEN_TESTING(BRACKET, "(", "(", 0, 1);
    TOKEN_TESTING(BRACKET, ")", ")", 0, 1);
    TOKEN_TESTING(BRACKET, "{", "{", 0, 1);
    TOKEN_TESTING(BRACKET, "}", "}", 0, 1);
    TOKEN_TESTING(BRACKET, "[", "[", 0, 1);
    TOKEN_TESTING(BRACKET, "]", "]", 0, 1);
}

TEST(TokenizerTest, TokenizeDelimiter)
{
    TOKEN_TESTING(DELIMITER, ",", ",", 0, 1);
    TOKEN_TESTING(DELIMITER, ";", ";", 0, 1);
}

TEST(TokenizerTest, TokenizeIdentifier)
{
    TOKEN_TESTING(IDENTIFIER, "myVar", "myVar", 0, 5);
    TOKEN_TESTING(IDENTIFIER, "_hidden", "_hidden", 0, 7);
    TOKEN_TESTING(IDENTIFIER, "abc123", "abc123", 0, 6);
}

TEST(TokenizerTest, InvalidIdentifier)
{
    TOKEN_TESTING(INVALID, "123abc", "123abc", 0, 6);
    TOKEN_TESTING(INVALID, "123abc423", "123abc423", 0, 9);
}

TEST(TokenizerTest, TokenizeOperators)
{
    TOKEN_TESTING(OPERATOR, "+", "+", 0, 1);
    TOKEN_TESTING(OPERATOR, "-", "-", 0, 1);
    TOKEN_TESTING(OPERATOR, "*", "*", 0, 1);
    TOKEN_TESTING(OPERATOR, "/", "/", 0, 1);
    TOKEN_TESTING(OPERATOR, "%", "%", 0, 1);
    TOKEN_TESTING(OPERATOR, "^", "^", 0, 1);
    TOKEN_TESTING(OPERATOR, "@", "@", 0, 1);
    TOKEN_TESTING(OPERATOR, "!", "!", 0, 1);
    TOKEN_TESTING(OPERATOR, "|", "|", 0, 1);
    TOKEN_TESTING(OPERATOR, "&", "&", 0, 1);

    TOKEN_TESTING(OPERATOR, "=", "=", 0, 1);

    TOKEN_TESTING(OPERATOR, "++", "++", 0, 2);
    TOKEN_TESTING(OPERATOR, "--", "--", 0, 2);

    TOKEN_TESTING(OPERATOR, "+=", "+=", 0, 2);
    TOKEN_TESTING(OPERATOR, "-=", "-=", 0, 2);
    TOKEN_TESTING(OPERATOR, "*=", "*=", 0, 2);
    TOKEN_TESTING(OPERATOR, "/=", "/=", 0, 2);
    TOKEN_TESTING(OPERATOR, "%=", "%=", 0, 2);
    TOKEN_TESTING(OPERATOR, "&=", "&=", 0, 2);
    TOKEN_TESTING(OPERATOR, "|=", "|=", 0, 2);
    TOKEN_TESTING(OPERATOR, "^=", "^=", 0, 2);
    TOKEN_TESTING(OPERATOR, "@=", "@=", 0, 2);

    TOKEN_TESTING(OPERATOR, ">", ">", 0, 1);
    TOKEN_TESTING(OPERATOR, "<", "<", 0, 1);
    TOKEN_TESTING(OPERATOR, "==", "==", 0, 2);
    TOKEN_TESTING(OPERATOR, "!=", "!=", 0, 2);
    TOKEN_TESTING(OPERATOR, ">=", ">=", 0, 2);
    TOKEN_TESTING(OPERATOR, "<=", "<=", 0, 2);

    TOKEN_TESTING(OPERATOR, "&&", "&&", 0, 2);
    TOKEN_TESTING(OPERATOR, "||", "||", 0, 2);
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

    LINE_PROPAGATION(AssertKEYWORDToken, tokens[index++], "const", -1, 5);
    LINE_PROPAGATION(AssertIDENTIFIERToken, tokens[index++], "pi", -1, 2);
    LINE_PROPAGATION(AssertOPERATORToken, tokens[index++], "=", -1, 1);
    LINE_PROPAGATION(AssertFLOATToken, tokens[index++], 3.14f, -1, 4);
    LINE_PROPAGATION(AssertDELIMITERToken, tokens[index++], ";", -1, 1);
    LINE_PROPAGATION(AssertKEYWORDToken, tokens[index++], "let", -1, 3);
    LINE_PROPAGATION(AssertIDENTIFIERToken, tokens[index++], "radius", -1, 6);
    LINE_PROPAGATION(AssertOPERATORToken, tokens[index++], "=", -1, 1);
    LINE_PROPAGATION(AssertINTEGERToken, tokens[index++], 10, -1, 2);
    LINE_PROPAGATION(AssertDELIMITERToken, tokens[index++], ";", -1, 1);
    LINE_PROPAGATION(AssertKEYWORDToken, tokens[index++], "if", -1, 2);
    LINE_PROPAGATION(AssertBRACKETToken, tokens[index++], "(", -1, 1);
    LINE_PROPAGATION(AssertIDENTIFIERToken, tokens[index++], "radius", -1, 6);
    LINE_PROPAGATION(AssertOPERATORToken, tokens[index++], ">", -1, 1);
    LINE_PROPAGATION(AssertINTEGERToken, tokens[index++], 0, -1, 1);
    LINE_PROPAGATION(AssertBRACKETToken, tokens[index++], ")", -1, 1);
    LINE_PROPAGATION(AssertBRACKETToken, tokens[index++], "{", -1, 1);
    LINE_PROPAGATION(AssertKEYWORDToken, tokens[index++], "let", -1, 3);
    LINE_PROPAGATION(AssertIDENTIFIERToken, tokens[index++], "area", -1, 4);
    LINE_PROPAGATION(AssertOPERATORToken, tokens[index++], "=", -1, 1);
    LINE_PROPAGATION(AssertIDENTIFIERToken, tokens[index++], "pi", -1, 2);
    LINE_PROPAGATION(AssertOPERATORToken, tokens[index++], "*", -1, 1);
    LINE_PROPAGATION(AssertIDENTIFIERToken, tokens[index++], "radius", -1, 6);
    LINE_PROPAGATION(AssertOPERATORToken, tokens[index++], "*", -1, 1);
    LINE_PROPAGATION(AssertIDENTIFIERToken, tokens[index++], "radius", -1, 6);
    LINE_PROPAGATION(AssertDELIMITERToken, tokens[index++], ";", -1, 1);
    LINE_PROPAGATION(AssertIDENTIFIERToken, tokens[index++], "print", -1, 5);
    LINE_PROPAGATION(AssertBRACKETToken, tokens[index++], "(", -1, 1);
    LINE_PROPAGATION(AssertSTRINGToken, tokens[index++], R"("Area is: ")", -1, 11);
    LINE_PROPAGATION(AssertOPERATORToken, tokens[index++], "+", -1, 1);
    LINE_PROPAGATION(AssertIDENTIFIERToken, tokens[index++], "area", -1, 4);
    LINE_PROPAGATION(AssertBRACKETToken, tokens[index++], ")", -1, 1);
    LINE_PROPAGATION(AssertDELIMITERToken, tokens[index++], ";", -1, 1);
    LINE_PROPAGATION(AssertBRACKETToken, tokens[index++], "}", -1, 1);
    LINE_PROPAGATION(AssertKEYWORDToken, tokens[index++], "else", -1, 4);
    LINE_PROPAGATION(AssertBRACKETToken, tokens[index++], "{", -1, 1);
    LINE_PROPAGATION(AssertIDENTIFIERToken, tokens[index++], "print", -1, 5);
    LINE_PROPAGATION(AssertBRACKETToken, tokens[index++], "(", -1, 1);
    LINE_PROPAGATION(AssertSTRINGToken, tokens[index++], R"("Invalid radius")", -1, 16);
    LINE_PROPAGATION(AssertBRACKETToken, tokens[index++], ")", -1, 1);
    LINE_PROPAGATION(AssertDELIMITERToken, tokens[index++], ";", -1, 1);
    LINE_PROPAGATION(AssertBRACKETToken, tokens[index++], "}", -1, 1);
}