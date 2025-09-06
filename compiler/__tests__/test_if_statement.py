import pytest  # type: ignore
from tokenizer import TokenType
from utils.parser_assertion import (
    AtomicAssertion,
    InvalidAtomicAssertion,
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


def test_if_without_expression():
    ast = Parser(
        """
if 
{
    x += 1;
}
"""
    )

    ast.Program.Compress()
    ast.Program.Parse()

    ProgramAssertion(
        IfStatementAsserion(
            condition=InvalidAtomicAssertion(),
            body=CodeBlockionAsserion(
                StatementAsserion(
                    OperationAssertion(
                        "+=",
                        left=AtomicAssertion(TokenType.IDENTIFIER, "x"),
                        right=AtomicAssertion(TokenType.INTEGER, 1),
                    ),
                ),
            ),
            error="Missing condition expression",
        ),
    ).Assert(ast.Program)


def test_if_without_round_brackets():
    ast = Parser(
        """ 
if x > 10 {
    y = 20;
}
"""
    )

    ast.Program.Compress()
    ast.Program.Parse()

    ProgramAssertion(
        IfStatementAsserion(
            condition=InvalidAtomicAssertion(),
            body=CodeBlockionAsserion(),
            error="Missing body block",
        ),
        StatementAsserion(
            OperationAssertion(
                ">",
                left=AtomicAssertion(TokenType.IDENTIFIER, "x"),
                right=AtomicAssertion(TokenType.INTEGER, 10),
            ),
            error="Missing semicolon at the end",
        ),
        CodeBlockionAsserion(
            StatementAsserion(
                OperationAssertion(
                    "=",
                    left=AtomicAssertion(TokenType.IDENTIFIER, "y"),
                    right=AtomicAssertion(TokenType.INTEGER, 20),
                ),
            ),
        ),
    ).Assert(ast.Program)
