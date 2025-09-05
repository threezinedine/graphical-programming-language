from abc import ABC, abstractmethod
from parser import (
    # Float,
    # IfStatement,
    # IfStatementBranch,
    # Integer,
    Node,
    # FunctionCall,
    # Parameter,
    # String,
)
from tokenizer import TokenValue


class DelayAssertion(ABC):
    @abstractmethod
    def Assert(self, node: Node) -> None:
        pass


# class ParameterAssertion(DelayAssertion):
#     def __init__(self, expectedValue: TokenValue) -> None:
#         self.expectedValue = expectedValue

#     def Assert(self, node: Node) -> None:
#         assert isinstance(node, Parameter), f"Expected Parameter, got {type(node)}"
#         assert (
#             isinstance(node.exp, Integer)
#             or isinstance(node.exp, Float)
#             or isinstance(node.exp, String)
#         ), f"Expected String, Float or Integer, got {type(node.exp)}"
#         assert (
#             node.exp.value == self.expectedValue
#         ), f"Expected {self.expectedValue}, got {node.exp.value}"


class StatementAssertion(DelayAssertion):
    def __init__(self) -> None:
        super().__init__()


class VariableDefinitionAssertion(StatementAssertion):
    def __init__(self, variableName: str, expectedValue: TokenValue) -> None:
        self.variableName = variableName
        self.expectedValue = expectedValue

    def Assert(self, node: Node) -> None:
        # assert isinstance(node, VariableDefinition)
        # assert node.variableName == self.variableName
        # assert node.expectedValue == self.expectedValue
        pass


class ConditionAssertion(DelayAssertion):
    def __init__(self, leftNode: Node, operator: str, rightNode: Node) -> None:
        super().__init__()
        self.leftNode = leftNode
        self.operator = operator
        self.rightNode = rightNode

    def Assert(self, node: Node) -> None:
        return super().Assert(node)


class BlockAssertion:
    def __init__(self, expressions: list[DelayAssertion]) -> None:
        self.expressions = expressions

    def Assert(self, nodes: list[Node]) -> None:
        assert len(nodes) == len(
            self.expressions
        ), f"Expected {len(self.expressions)} expressions, got {len(nodes)}"
        for index in range(len(self.expressions)):
            self.expressions[index].Assert(nodes[index])


# class IfStatementBranchAssertion(DelayAssertion):
#     def __init__(
#         self,
#         condition: ConditionAssertion | None = None,
#         block: BlockAssertion | None = None,
#     ) -> None:
#         super().__init__()
#         self.condition = condition
#         self.block = block

#     def Assert(self, node: Node) -> None:
#         assert isinstance(
#             node, IfStatementBranch
#         ), f"Expected IfStatementBranch, got {type(node)}"
#         if self.condition:
#             self.condition.Assert(node.condition)
#         if self.block:
#             self.block.Assert(node.block)


# class IfStatementAssertion(DelayAssertion):
#     def __init__(self, branches: list[IfStatementBranchAssertion]) -> None:
#         super().__init__()
#         self.branches = branches

#     def Assert(self, node: Node) -> None:
#         assert isinstance(node, IfStatement), f"Expected IfStatement, got {type(node)}"
#         assert len(node.branches) == len(
#             self.branches
#         ), f"Expected {len(self.branches)} branches, got {len(node.branches)}"
#         for branchIndex in range(len(self.branches)):
#             self.branches[branchIndex].Assert(node.branches[branchIndex])
#         return super().Assert(node)


# class FunctionCallAssertion(DelayAssertion):
#     def __init__(
#         self,
#         functionName: str,
#         parameters: list[ParameterAssertion],
#     ) -> None:
#         self.functionName = functionName
#         self.parameters = parameters

#     def Assert(self, node: Node) -> None:
#         assert isinstance(
#             node, FunctionCall
#         ), f"Expected FunctionCall, got {type(node)}"
#         assert (
#             node.functionName == self.functionName
#         ), f"Expected function name {self.functionName}, got {node.functionName}"
#         assert len(node.parameters) == len(
#             self.parameters
#         ), f"Expected {len(self.parameters)} parameters, got {len(node.parameters)}"
#         for index in range(len(self.parameters)):
#             self.parameters[index].Assert(node.parameters[index])
