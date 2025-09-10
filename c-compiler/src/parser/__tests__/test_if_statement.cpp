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
                        ATOMIC_ASSERTION(TokenType::INTEGER, u32(5))))),
            BLOCK_ASSERTION()));
}

TEST(IfStatementTest, NestedIfStatement)
{
    PARSE_DEFINE(R"(
if (true) {
    if (false) {
        a = 5;
    }   
}
)");

    PROGRAM_ASSERTION(
        IF_STATEMENT_ASSERTION(
            EXPRESSION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::BOOLEAN, NTT_TRUE)),
            BLOCK_ASSERTION(
                IF_STATEMENT_ASSERTION(
                    EXPRESSION_ASSERTION(
                        ATOMIC_ASSERTION(TokenType::BOOLEAN, NTT_FALSE)),
                    BLOCK_ASSERTION(
                        STATEMENT_ASSERTION(
                            OPERATION_ASSERTION(
                                ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                                "=",
                                ATOMIC_ASSERTION(TokenType::INTEGER, u32(5))))),
                    BLOCK_ASSERTION())),
            BLOCK_ASSERTION()));
}

TEST(IfStatementTest, ElseBlock)
{
    PARSE_DEFINE(R"(
if (true) {
    a = 5;
} else {
    a = 10;
}
)");

    PROGRAM_ASSERTION(
        IF_STATEMENT_ASSERTION(
            EXPRESSION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::BOOLEAN, NTT_TRUE)),
            BLOCK_ASSERTION(
                STATEMENT_ASSERTION(
                    OPERATION_ASSERTION(
                        ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                        "=",
                        ATOMIC_ASSERTION(TokenType::INTEGER, u32(5))))),
            BLOCK_ASSERTION(
                STATEMENT_ASSERTION(
                    OPERATION_ASSERTION(
                        ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                        "=",
                        ATOMIC_ASSERTION(TokenType::INTEGER, u32(10)))))));
}

TEST(IfStatementTest, FullyNestedIfElse)
{
    PARSE_DEFINE(R"(
if (true) {
    if (false) {
        a = 5;
    } else {
        a = 10;
    }
} else {
    a = 15;
})");

    PROGRAM_ASSERTION(
        IF_STATEMENT_ASSERTION(
            EXPRESSION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::BOOLEAN, NTT_TRUE)),
            BLOCK_ASSERTION(
                IF_STATEMENT_ASSERTION(
                    EXPRESSION_ASSERTION(
                        ATOMIC_ASSERTION(TokenType::BOOLEAN, NTT_FALSE)),
                    BLOCK_ASSERTION(
                        STATEMENT_ASSERTION(
                            OPERATION_ASSERTION(
                                ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                                "=",
                                ATOMIC_ASSERTION(TokenType::INTEGER, u32(5))))),
                    BLOCK_ASSERTION(
                        STATEMENT_ASSERTION(
                            OPERATION_ASSERTION(
                                ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                                "=",
                                ATOMIC_ASSERTION(TokenType::INTEGER, u32(10))))))),
            BLOCK_ASSERTION(
                STATEMENT_ASSERTION(
                    OPERATION_ASSERTION(
                        ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                        "=",
                        ATOMIC_ASSERTION(TokenType::INTEGER, u32(15)))))));
}

TEST(IfStatementTest, TestElseIf)
{
    PARSE_DEFINE(R"(
if (a == 3) {
    a = 5;
} else if (a == 4) {
    a = 10;
} else {
    a = 15;
})");

    PROGRAM_ASSERTION(
        IF_STATEMENT_ASSERTION(
            EXPRESSION_ASSERTION(
                OPERATION_ASSERTION(
                    ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                    "==",
                    ATOMIC_ASSERTION(TokenType::INTEGER, u32(3)))),
            BLOCK_ASSERTION(
                STATEMENT_ASSERTION(
                    OPERATION_ASSERTION(
                        ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                        "=",
                        ATOMIC_ASSERTION(TokenType::INTEGER, u32(5))))),
            IF_STATEMENT_ASSERTION(
                EXPRESSION_ASSERTION(
                    OPERATION_ASSERTION(
                        ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                        "==",
                        ATOMIC_ASSERTION(TokenType::INTEGER, u32(4)))),
                BLOCK_ASSERTION(
                    STATEMENT_ASSERTION(
                        OPERATION_ASSERTION(
                            ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                            "=",
                            ATOMIC_ASSERTION(TokenType::INTEGER, u32(10))))),
                BLOCK_ASSERTION(
                    STATEMENT_ASSERTION(
                        OPERATION_ASSERTION(
                            ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                            "=",
                            ATOMIC_ASSERTION(TokenType::INTEGER, u32(15))))))));
}

TEST(IfStatementTest, MissingCondition)
{
    PARSE_DEFINE("if { a = 5; }");

    PROGRAM_ASSERTION(
        IF_STATEMENT_ASSERTION_ERR(
            ErrorType::MISSING_CONDITION,
            INVALID_ASSERTION(),
            BLOCK_ASSERTION(
                STATEMENT_ASSERTION(
                    OPERATION_ASSERTION(
                        ATOMIC_ASSERTION(TokenType::IDENTIFIER, "a"),
                        "=",
                        ATOMIC_ASSERTION(TokenType::INTEGER, u32(5))))),
            BLOCK_ASSERTION()));
}

TEST(IfStatement, MissingBlock)
{
    PARSE_DEFINE_P(R"(
if (testing)
)");

    PROGRAM_ASSERTION(
        IF_STATEMENT_ASSERTION_ERR(
            ErrorType::MISSING_BLOCK,
            EXPRESSION_ASSERTION(
                ATOMIC_ASSERTION(TokenType::IDENTIFIER, "testing")),
            BLOCK_ASSERTION(),
            BLOCK_ASSERTION()));
}