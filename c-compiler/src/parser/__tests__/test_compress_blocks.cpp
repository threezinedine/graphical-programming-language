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