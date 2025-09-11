#include "test_common.h"
#include "parser/blockNode.h"
#include "assertions.h"

TEST(FunctionCallTest, SimpleFunctionCall)
{
    PARSE_DEFINE("foo();");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            FUNCTION_CALL_ASSERTION(
                FUNCTION_CALL_NAME_ASSERTION("foo"))));
}