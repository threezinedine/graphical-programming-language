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
"""
    )

    assert len(ast.Nodes) == 1
    ProgramAssertion(
        [
            FunctionCallAssertion(
                "print",
                [ParameterAssertion('"Hello World"')],
            ),
        ]
    ).Assert(ast.Nodes)
