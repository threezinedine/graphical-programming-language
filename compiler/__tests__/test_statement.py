import pytest  # type: ignore
from tokenizer import TokenType
from utils.parser_assertion import (
    AtomicAssertion,
    OperationAssertion,
    ProgramAssertion,
    StatementAsserion,
)
from parser import Parser


def test_parse_simple_statement():
    ast = Parser("x = 42;")

    ast.Program.Compress()
    ast.Program.Parse()

    print(ast.Program)

    ProgramAssertion(
        StatementAsserion(
            OperationAssertion(
                "=",
                left=AtomicAssertion(TokenType.IDENTIFIER, "x"),
                right=AtomicAssertion(TokenType.INTEGER, 42),
            ),
        ),
    ).Assert(ast.Program)


def test_parse_final_statement_without_semicolon():
    ast = Parser("x = 42")

    ast.Program.Compress()
    ast.Program.Parse()

    print(ast.Program)

    ProgramAssertion(
        StatementAsserion(
            OperationAssertion(
                "=",
                left=AtomicAssertion(TokenType.IDENTIFIER, "x"),
                right=AtomicAssertion(TokenType.INTEGER, 42),
            ),
            error="Missing semicolon at the end",
        ),
    ).Assert(ast.Program)
