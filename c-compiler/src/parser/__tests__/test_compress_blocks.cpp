#include "test_common.h"
#include "parser/blockNode.h"
#include "assertions.h"

using namespace ntt;

TEST(BlockNodeCompressTest, CreateWithContent)
{
    COMPRESS_ONLY_DEFINE("23");

    PROGRAM_ASSERTION(
        ATOMIC_ASSERTION(TokenType::INTEGER, u32(23)));
}

TEST(BlockNodeCompressTest, HasBlockCode)
{
    COMPRESS_ONLY_DEFINE("3 + (2 + 4)");

    PROGRAM_ASSERTION(
        ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
        ATOMIC_ASSERTION(TokenType::OPERATOR, "+"),
        EXPRESSION_ASSERTION(
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(2)),
            ATOMIC_ASSERTION(TokenType::OPERATOR, "+"),
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(4))));
}

TEST(BlockNodeCompressTest, NestedBlockCode)
{
    COMPRESS_ONLY_DEFINE("3 + (2 + (5 - 3))");

    PROGRAM_ASSERTION(
        ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
        ATOMIC_ASSERTION(TokenType::OPERATOR, "+"),
        EXPRESSION_ASSERTION(
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(2)),
            ATOMIC_ASSERTION(TokenType::OPERATOR, "+"),
            EXPRESSION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(5)),
                ATOMIC_ASSERTION(TokenType::OPERATOR, "-"),
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)))));
}

TEST(BlockNodeCompressTest, NestedBlockCodeWithSquareBrackets)
{
    COMPRESS_ONLY_DEFINE("3 + [2 + (5 - 3)]");

    PROGRAM_ASSERTION(
        ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
        ATOMIC_ASSERTION(TokenType::OPERATOR, "+"),
        INDEX_ASSERTION(
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(2)),
            ATOMIC_ASSERTION(TokenType::OPERATOR, "+"),
            EXPRESSION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(5)),
                ATOMIC_ASSERTION(TokenType::OPERATOR, "-"),
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)))));
}

TEST(BlockNodeCompressTest, OnlySquareBrackets)
{
    COMPRESS_ONLY_DEFINE("[2.2 + 4]");

    PROGRAM_ASSERTION(
        INDEX_ASSERTION(
            ATOMIC_ASSERTION(TokenType::FLOAT, 2.2f),
            ATOMIC_ASSERTION(TokenType::OPERATOR, "+"),
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(4))));
}

TEST(BlockNodeCompressTest, OnlyBrackets)
{
    COMPRESS_ONLY_DEFINE("{ print(\"Hello, World!\"); }");

    PROGRAM_ASSERTION(
        BLOCK_ASSERTION(
            ATOMIC_ASSERTION(TokenType::IDENTIFIER, "print"),
            EXPRESSION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::STRING, "\"Hello, World!\"")),
            ATOMIC_ASSERTION(TokenType::DELIMITER, ";")));
}

TEST(BlockNodeCompressTest, MixBrackets)
{
    COMPRESS_ONLY_DEFINE(R"(
if (tokens[0] == "a")
{
    print("Hello, World!", 4);
}
)");

    PROGRAM_ASSERTION(
        ATOMIC_ASSERTION(TokenType::KEYWORD, "if"),
        EXPRESSION_ASSERTION(
            ATOMIC_ASSERTION(TokenType::IDENTIFIER, "tokens"),
            INDEX_ASSERTION(
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(0))),
            ATOMIC_ASSERTION(TokenType::OPERATOR, "=="),
            ATOMIC_ASSERTION(TokenType::STRING, "\"a\"")),
        BLOCK_ASSERTION(
            ATOMIC_ASSERTION(TokenType::IDENTIFIER, "print"),
            EXPRESSION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::STRING, "\"Hello, World!\""),
                ATOMIC_ASSERTION(TokenType::DELIMITER, ","),
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(4))),
            ATOMIC_ASSERTION(TokenType::DELIMITER, ";")));
}

TEST(BlockNodeCompressTest, OnlyCloseBracket)
{
    COMPRESS_ONLY_DEFINE("print(\"Testing\")}");

    PROGRAM_ASSERTION(
        ATOMIC_ASSERTION(TokenType::IDENTIFIER, "print"),
        EXPRESSION_ASSERTION(
            ATOMIC_ASSERTION(TokenType::STRING, "\"Testing\"")),
        ATOMIC_ASSERTION(TokenType::BRACKET, "}"));
}

TEST(BlockNodeCompressTest, MissingCloseBracket)
{
    COMPRESS_ONLY_DEFINE("{print(\"Testing\")");

    PROGRAM_ASSERTION(
        BLOCK_ASSERTION(
            ATOMIC_ASSERTION(TokenType::IDENTIFIER, "print"),
            EXPRESSION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::STRING, "\"Testing\""))));
}