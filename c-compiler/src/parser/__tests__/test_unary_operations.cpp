#include "test_common.h"
#include "parser/blockNode.h"
#include "assertions.h"

TEST(UnaryOperationsTest, SimpleUnaryPlus)
{
    PARSE_DEFINE("!true");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            UNARY_ASSERTION(
                "!",
                ATOMIC_ASSERTION(TokenType::BOOLEAN, NTT_TRUE))));
}

TEST(UnaryOperationsTest, MissingOperand)
{
    PARSE_DEFINE("! ");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            UNARY_ASSERTION_ERR(
                ErrorType::MISSING_RIGHT_OPERAND,
                "!",
                INVALID_ASSERTION())));
}

TEST(UnaryOperationsTest, NestedUnaryOperand)
{
    PARSE_DEFINE("!!test");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            UNARY_ASSERTION(
                "!",
                UNARY_ASSERTION(
                    "!",
                    ATOMIC_ASSERTION(TokenType::IDENTIFIER, "test")))));
}

TEST(UnaryOperationsTest, UnaryWithExpressionOperand)
{
    PARSE_DEFINE("!(true)");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            UNARY_ASSERTION(
                "!",
                EXPRESSION_ASSERTION(
                    ATOMIC_ASSERTION(TokenType::BOOLEAN, NTT_TRUE)))));
}

TEST(UnaryOperationsTest, UnaryWithInvalidOperand)
{
    PARSE_DEFINE("!{ }");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            UNARY_ASSERTION_ERR(
                ErrorType::MISSING_RIGHT_OPERAND,
                "!",
                INVALID_ASSERTION())),
        BLOCK_ASSERTION());
}