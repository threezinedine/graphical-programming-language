import pytest  # type: ignore
from parser import Parser
from utils.parser_assertion import (
    ParameterAssertion,
    FunctionCallAssertion,
    ProgramAssertion,
    VariableDefinitionAssertion,
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


def test_parse_variable_definition():
    ast = Parser(
        """
int x = 5;
auto y = 10.5;
"""
    )

    ProgramAssertion(
        [
            VariableDefinitionAssertion("x", 5),
            VariableDefinitionAssertion("y", 10.5),
        ]
    ).Assert(ast.Nodes)
