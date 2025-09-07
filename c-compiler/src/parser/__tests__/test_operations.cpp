#include "test_common.h"
#include "parser/blockNode.h"
#include "assertions.h"

TEST(OperationTest, SimpleAddition)
{
    PARSE_DEFINE("3 + 4");

    PROGRAM_ASSERTION(
        OPERATION_ASSERTION(
            ATOMIC_ASSERTION(TokenType::OPERATOR, "+"),
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(4))));
}

TEST(OperationTest, MultipleOperations)
{
    PARSE_DEFINE("3 + 4 - 2");

    PROGRAM_ASSERTION(
        OPERATION_ASSERTION(
            ATOMIC_ASSERTION(TokenType::OPERATOR, "-"),
            OPERATION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::OPERATOR, "+"),
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(4))),
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(2))));
}