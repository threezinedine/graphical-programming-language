#include "test_common.h"
#include "parser/parser.h"
#include "assertions.h"

using namespace ntt;

TEST(CodeBlockTest, DISABLED_impleCodeBlock)
{
    PARSE_DEFINE("{ int a = 5; }");

    PROGRAM_ASSERTION(
        BLOCK_ASSERTION(
            ATOMIC_ASSERTION(TokenType::KEYWORD, "int"),
            ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
            OPERATION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                "=",
                ATOMIC_ASSERTION(TokenType::INTEGER, u32(5)))));
}