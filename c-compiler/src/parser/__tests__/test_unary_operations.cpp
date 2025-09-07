#include "test_common.h"
#include "parser/blockNode.h"
#include "assertions.h"

TEST(UnaryOperationsTest, SimpleUnaryPlus)
{
    PARSE_DEFINE("!true");

    PROGRAM_ASSERTION(
        UNARY_ASSERTION(
            ATOMIC_ASSERTION(TokenType::OPERATOR, "!"),
            ATOMIC_ASSERTION(TokenType::BOOLEAN, NTT_TRUE)));
}

TEST(UnaryOperationsTest, MissingOperand)
{
    PARSE_DEFINE("! ");

    PROGRAM_ASSERTION(
        UNARY_ASSERTION_ERR(
            ErrorType::MISSING_RIGHT_OPERAND,
            ATOMIC_ASSERTION(TokenType::OPERATOR, "!"),
            INVALID_ASSERTION()));
}

TEST(UnaryOperationsTest, NestedUnaryOperand)
{
    PARSE_DEFINE("!!test");

    PROGRAM_ASSERTION(
        UNARY_ASSERTION(
            ATOMIC_ASSERTION(TokenType::OPERATOR, "!"),
            UNARY_ASSERTION(
                ATOMIC_ASSERTION(TokenType::OPERATOR, "!"),
                ATOMIC_ASSERTION(TokenType::IDENTIFIER, "test"))));
}