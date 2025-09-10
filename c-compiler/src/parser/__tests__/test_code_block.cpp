#include "test_common.h"
#include "parser/parser.h"
#include "assertions.h"

using namespace ntt;

TEST(CodeBlockTest, SimpleCodeBlock)
{
    PARSE_DEFINE("{ a = 5; }");

    PROGRAM_ASSERTION(
        BLOCK_ASSERTION(
            STATEMENT_ASSERTION(
                OPERATION_ASSERTION(
                    ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                    "=",
                    ATOMIC_ASSERTION(TokenType::INTEGER, u32(5))))));
}

TEST(CodeBlockTest, MissingDelimter)
{
    PARSE_DEFINE("{ a = 5 }");

    PROGRAM_ASSERTION(
        BLOCK_ASSERTION(
            STATEMENT_ASSERTION_ERR(
                ErrorType::MISSING_SEMICOLON,
                OPERATION_ASSERTION(
                    ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                    "=",
                    ATOMIC_ASSERTION(TokenType::INTEGER, u32(5))))));
}