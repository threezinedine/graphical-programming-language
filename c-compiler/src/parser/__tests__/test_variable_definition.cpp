#include "test_common.h"
#include "assertions.h"
#include "parser/blockNode.h"

using namespace ntt;

TEST(VariableDefinitionTest, SimpleVariableDefinition)
{
    PARSE_DEFINE("let a: number;");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            VARIABLE_DEFINITION_ASSERTION(
                "let", "a", "integer", ATOMIC_ASSERTION(TokenType::INTEGER, u32(0)))));
}

TEST(VariableDefinitionTest, ConstVariableDefinition)
{
    PARSE_DEFINE("const b: string;");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            VARIABLE_DEFINITION_ASSERTION(
                "const", "b", "string", ATOMIC_ASSERTION(TokenType::STRING, ""))));
}

TEST(VariableDefinitionTest, DefineWithDefaultValue)
{
    PARSE_DEFINE("let c: number = 5;");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            VARIABLE_DEFINITION_ASSERTION(
                "let", "c", "integer", ATOMIC_ASSERTION(TokenType::INTEGER, u32(5)))));
}

TEST(VariableDefinitionTest, DefineWithStringDefaultValue)
{
    PARSE_DEFINE("let d: string = \"hello\";");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            VARIABLE_DEFINITION_ASSERTION(
                "let", "d", "string", ATOMIC_ASSERTION(TokenType::STRING, "hello"))));
}

TEST(VariableDefinitionTest, NoneTypeDefine)
{
    PARSE_DEFINE("let e;");

    PROGRAM_ASSERTION(
        STATEMENT_ASSERTION(
            VARIABLE_DEFINITION_ASSERTION(
                "let", "e", "any", ATOMIC_ASSERTION(TokenType::NONE, "null"))));
}