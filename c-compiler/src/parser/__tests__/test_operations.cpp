#include "test_common.h"
#include "parser/blockNode.h"
#include "assertions.h"

TEST(OperationTest, SimpleAddition)
{
    PARSE_DEFINE("3 + 4");

    PROGRAM_ASSERTION(
        OPERATION_ASSERTION(
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
            "+",
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(4))));
}

TEST(OperationTest, MultipleOperations)
{
    PARSE_DEFINE("3 + 4 - 2");

    PROGRAM_ASSERTION(
        OPERATION_ASSERTION(
            OPERATION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
                "+",
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(4))),
            "-",
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(2))));
}

TEST(OperationTest, MultipleAndDivider)
{
    PARSE_DEFINE("3 * 4 / 2");

    PROGRAM_ASSERTION(
        OPERATION_ASSERTION(
            OPERATION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
                "*",
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(4))),
            "/",
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(2))));
}

TEST(OperationTest, MixedOperations)
{
    PARSE_DEFINE("3 + 4 * 2 - 1 / 5");

    PROGRAM_ASSERTION(
        OPERATION_ASSERTION(
            OPERATION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
                "+",
                OPERATION_ASSERTION(
                    ATOMIC_ASSERTION(TokenType::INTEGER, u32(4)),
                    "*",
                    ATOMIC_ASSERTION(TokenType::INTEGER, u32(2)))),
            "-",
            OPERATION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(1)),
                "/",
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(5)))));
}

TEST(OperationTest, PowerOperation)
{
    PARSE_DEFINE("3 * 4 ^ 2");

    PROGRAM_ASSERTION(
        OPERATION_ASSERTION(
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
            "*",
            OPERATION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(4)),
                "^",
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(2)))));
}

TEST(OperationTest, OperaionWithBrackets)
{
    PARSE_DEFINE("(3 + 4) * 2");

    PROGRAM_ASSERTION(
        OPERATION_ASSERTION(
            EXPRESSION_ASSERTION(
                OPERATION_ASSERTION(
                    ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
                    "+",
                    ATOMIC_ASSERTION(TokenType::INTEGER, u32(4)))),
            "*",
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(2))));
}

TEST(OperationTest, MissingRightOperand)
{
    PARSE_DEFINE("3 + ");

    PROGRAM_ASSERTION(
        OPERATION_ASSERTION_ERR(
            ErrorType::MISSING_RIGHT_OPERAND,
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
            "+",
            INVALID_ASSERTION()));
}

TEST(OperationTest, MixedMissingRightOperand)
{
    PARSE_DEFINE("3 * + 4");

    PROGRAM_ASSERTION(
        OPERATION_ASSERTION(
            OPERATION_ASSERTION_ERR(
                ErrorType::MISSING_RIGHT_OPERAND,
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
                "*",
                INVALID_ASSERTION()),
            "+",
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(4))));
}

TEST(OperationTest, MissingLeftOperand)
{
    PARSE_DEFINE("+ 4");

    PROGRAM_ASSERTION(
        OPERATION_ASSERTION_ERR(
            ErrorType::MISSING_LEFT_OPERAND,
            INVALID_ASSERTION(),
            "+",
            ATOMIC_ASSERTION(TokenType::INTEGER, u32(4))));
}