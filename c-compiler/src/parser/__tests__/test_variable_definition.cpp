#include "test_common.h"
#include "assertions.h"
#include "parser/blockNode.h"

using namespace ntt;

TEST(VariableDefinitionTest, SimpleVariableDefinition)
{
    PARSE_DEFINE("let a: number;");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            LET_VARIABLE_DEFINITION_ASSERTION(
                "a", "integer")));
}