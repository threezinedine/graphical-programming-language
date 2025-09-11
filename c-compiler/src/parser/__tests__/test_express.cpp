#include "test_common.h"
#include "parser/parser.h"
#include "assertions.h"

using namespace ntt;

TEST(ExpressionTest, SimpleExpression)
{
    PARSE_DEFINE("(3 + 4)");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            EXPRESSION_ASSERTION(
                OPERATION_ASSERTION(
                    ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
                    "+",
                    ATOMIC_ASSERTION(TokenType::INTEGER, u32(4))))));
}

TEST(ExpressionTest, WithDelimiter)
{
    PARSE_DEFINE("(a, 4)");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            EXPRESSION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(4)))));
}

TEST(ExpressionTest, NestedExpressionWithFunctionCall)
{
    PARSE_DEFINE("(a, foo())");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            EXPRESSION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                FUNCTION_CALL_ASSERTION(
                    ATOMIC_ASSERTION(TokenType::IDENTIFIER, "foo")))));
}