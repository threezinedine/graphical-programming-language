from abc import ABC, abstractmethod
from parser import Node, FunctionCall


class DelayAssertion(ABC):
    @abstractmethod
    def Assert(self, node: Node) -> None:
        pass


class ParameterAssertion(DelayAssertion):
    def __init__(self, expectedValue: str) -> None:
        self.expectedValue = expectedValue

    def Assert(self, node: Node) -> None:
        # assert isinstance(node, Parameter)
        # assert node.value == self.expectedValue
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
        assert isinstance(node, FunctionCall)
        assert node.functionName == self.functionName
        assert len(node.parameters) == len(self.parameters)
        for index in range(len(self.parameters)):
            self.parameters[index].Assert(node.parameters[index])


class ProgramAssertion:
    def __init__(self, expressions: list[DelayAssertion]) -> None:
        super().__init__()
        self.expressions = expressions

    def Assert(self, nodes: list[Node]) -> None:
        assert len(nodes) == len(self.expressions)
        for index in range(len(self.expressions)):
            self.expressions[index].Assert(nodes[index])
