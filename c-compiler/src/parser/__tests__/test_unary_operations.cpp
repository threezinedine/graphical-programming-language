#include "test_common.h"
#include "parser/blockNode.h"
#include "assertions.h"

TEST(UnaryOperationsTest, SimpleUnaryPlus)
{
    PARSE_DEFINE("!true");

    PROGRAM_ASSERTION(
        UNARY_ASSERTION(
            ATOMIC_ASSERTION(TokenType::OPERATOR, "!"),
            ATOMIC_ASSERTION(TokenType::BOOLEAN, "true")));
}