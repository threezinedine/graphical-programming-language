import pytest  # type: ignore
from tokenizer import TokenType
from utils.parser_assertion import (
    AtomicAssertion,
    OperationAssertion,
    ProgramAssertion,
    StatementAsserion,
    IfStatementAsserion,
    CodeBlockionAsserion,
    ExpressBlockionAsserion,
)
from parser import Parser


def test_simple_if_statement():
    ast = Parser(
        """
if (x > 10) {
    y = 20;
}
"""
    )

    ast.Program.Compress()
    ast.Program.Parse()

    print(ast.Program)

    ProgramAssertion(
        IfStatementAsserion(
            condition=ExpressBlockionAsserion(
                OperationAssertion(
                    ">",
                    left=AtomicAssertion(TokenType.IDENTIFIER, "x"),
                    right=AtomicAssertion(TokenType.INTEGER, 10),
                ),
            ),
            body=CodeBlockionAsserion(
                StatementAsserion(
                    OperationAssertion(
                        "=",
                        left=AtomicAssertion(TokenType.IDENTIFIER, "y"),
                        right=AtomicAssertion(TokenType.INTEGER, 20),
                    ),
                ),
            ),
        ),
    ).Assert(ast.Program)


def test_if_without_body():
    ast = Parser(
        """
if (x > 10)
"""
    )

    ast.Program.Compress()
    ast.Program.Parse()

    print(ast.Program)

    ProgramAssertion(
        IfStatementAsserion(
            condition=ExpressBlockionAsserion(
                OperationAssertion(
                    ">",
                    left=AtomicAssertion(TokenType.IDENTIFIER, "x"),
                    right=AtomicAssertion(TokenType.INTEGER, 10),
                ),
            ),
            body=CodeBlockionAsserion(),
            error="Missing body block",
        ),
    ).Assert(ast.Program)
