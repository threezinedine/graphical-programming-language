import pytest  # type: ignore
from parser import Parser
from utils.parser_assertion import (
    ParameterAssertion,
    FunctionCallAssertion,
    ProgramAssertion,
)


def test_parse_simple_program():
    ast = Parser(
        """
print("Hello World");
print(23.4, "testing");
"""
    )

    ProgramAssertion(
        [
            FunctionCallAssertion(
                "print",
                [ParameterAssertion('"Hello World"')],
            ),
            FunctionCallAssertion(
                "print",
                [ParameterAssertion(23.4), ParameterAssertion('"testing"')],
            ),
        ]
    ).Assert(ast.Nodes)
