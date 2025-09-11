#include "test_common.h"
#include "parser/blockNode.h"
#include "assertions.h"

TEST(FunctionCallTest, SimpleFunctionCall)
{
    PARSE_DEFINE("foo();");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            FUNCTION_CALL_ASSERTION(
                "foo")));
}

TEST(FunctionCallTest, FunctionCallWithArguments)
{
    PARSE_DEFINE_P("foo(3, 4);");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            FUNCTION_CALL_ASSERTION(
                "foo",
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)),
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(4)))));
}

TEST(FunctionCallTest, NestedFunctionCall)
{
    PARSE_DEFINE_P("foo(bar(3), 4);");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            FUNCTION_CALL_ASSERTION(
                "foo",
                FUNCTION_CALL_ASSERTION(
                    "bar",
                    ATOMIC_ASSERTION(TokenType::INTEGER, u32(3))),
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(4)))));
}