from abc import ABC, abstractmethod
from parser import Float, Integer, Node, FunctionCall, Parameter, String
from tokenizer import TokenValue


class DelayAssertion(ABC):
    @abstractmethod
    def Assert(self, node: Node) -> None:
        pass


class ParameterAssertion(DelayAssertion):
    def __init__(self, expectedValue: TokenValue) -> None:
        self.expectedValue = expectedValue

    def Assert(self, node: Node) -> None:
        assert isinstance(node, Parameter), f"Expected Parameter, got {type(node)}"
        assert (
            isinstance(node.exp, Integer)
            or isinstance(node.exp, Float)
            or isinstance(node.exp, String)
        ), f"Expected String, Float or Integer, got {type(node.exp)}"
        assert (
            node.exp.value == self.expectedValue
        ), f"Expected {self.expectedValue}, got {node.exp.value}"


class VariableDefinitionAssertion(DelayAssertion):
    def __init__(self, variableName: str, expectedValue: TokenValue) -> None:
        self.variableName = variableName
        self.expectedValue = expectedValue

    def Assert(self, node: Node) -> None:
        # assert isinstance(node, VariableDefinition)
        # assert node.variableName == self.variableName
        # assert node.expectedValue == self.expectedValue
        pass


class FunctionCallAssertion(DelayAssertion):
    def __init__(
        self,
        functionName: str,
        parameters: list[ParameterAssertion],
    ) -> None:
        self.functionName = functionName
        self.parameters = parameters

    def Assert(self, node: Node) -> None:
        assert isinstance(
            node, FunctionCall
        ), f"Expected FunctionCall, got {type(node)}"
        assert (
            node.functionName == self.functionName
        ), f"Expected function name {self.functionName}, got {node.functionName}"
        assert len(node.parameters) == len(
            self.parameters
        ), f"Expected {len(self.parameters)} parameters, got {len(node.parameters)}"
        for index in range(len(self.parameters)):
            self.parameters[index].Assert(node.parameters[index])


class ProgramAssertion:
    def __init__(self, expressions: list[DelayAssertion]) -> None:
        super().__init__()
        self.expressions = expressions

    def Assert(self, nodes: list[Node]) -> None:
        assert len(nodes) == len(
            self.expressions
        ), f"Expected {len(self.expressions)} expressions, got {len(nodes)}"
        for index in range(len(self.expressions)):
            self.expressions[index].Assert(nodes[index])
