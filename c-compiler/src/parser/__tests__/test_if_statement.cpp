#include "test_common.h"
#include "parser/blockNode.h"
#include "assertions.h"

using namespace ntt;

TEST(IfStatementTest, SimpleIfStatement)
{
    PARSE_DEFINE("if (true) { a = 5; }");

    PROGRAM_ASSERTION(
        IF_STATEMENT_ASSERTION(
            EXPRESSION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::BOOLEAN, NTT_TRUE)),
            BLOCK_ASSERTION(
                STATEMENT_ASSERTION(
                    OPERATION_ASSERTION(
                        ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                        "=",
                        ATOMIC_ASSERTION(TokenType::INTEGER, u32(5)))))));
}