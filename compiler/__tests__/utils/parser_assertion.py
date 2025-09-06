from abc import ABC, abstractmethod
from tokenizer import TokenType, Token, TokenValue
from parser import (
    AtomicNode,
    BlockTypeNode,
    InvalidNode,
    Node,
    NodeType,
    OperationNode,
    UnaryOperationNode,
)


class DelayAssertion(ABC):
    @abstractmethod
    def Assert(self, node: Node) -> None:
        pass


class AtomicAssertion(DelayAssertion):
    def __init__(self, tokenType: TokenType, value: TokenValue) -> None:
        self.value = value
        self.tokenType = tokenType

    def Assert(self, node: Node) -> None:
        assert len(node) == 1, f"Atomic should have 1 node, but got {len(node)}"
        assert isinstance(node, AtomicNode), "Node should be AtomicNode"
        assert (
            node.token.Type.value == self.tokenType.value
        ), f"Atomic type should be {self.tokenType.name}, but got {node.token.Type.name}"
        assert (
            node.token.Value == self.value
        ), f"Atomic value should be {self.value}, but got {node.token.Value}"


class InvalidAtomicAssertion(DelayAssertion):
    def Assert(self, node: Node) -> None:
        assert len(node) == 0, f"Atomic should have 0 node, but got {len(node)}"
        assert isinstance(node, InvalidNode), "Node should be AtomicNode"


class OperationAssertion(DelayAssertion):
    def __init__(
        self,
        operator: str,
        left: DelayAssertion,
        right: DelayAssertion,
        error: str | None = None,
    ) -> None:
        self.operator = operator
        self.left = left
        self.right = right
        self.error = error

    def Assert(self, node: Node) -> None:
        assert isinstance(
            node, OperationNode
        ), f"Node should be OperationNode, but got {type(node)}"

        assert isinstance(
            node.Operator, Token
        ), f"Operation operator should be Token, but got {type(node.Operator)}"
        assert (
            node.Operator.Value == self.operator
        ), f"Operator should be {self.operator}, but got {node.Operator.Value}"

        if self.error is not None:
            assert (
                node.Error == self.error
            ), f"Error should be {self.error}, but got {node.Error}"

        self.left.Assert(node.Left)
        self.right.Assert(node.Right)


class UnaryOperationAssertion(DelayAssertion):
    def __init__(
        self,
        operator: str,
        exp: DelayAssertion,
        error: str | None = None,
    ) -> None:
        self.operator = operator
        self.exp = exp
        self.error = error

    def Assert(self, node: Node) -> None:
        assert isinstance(
            node, UnaryOperationNode
        ), f"Node should be UnaryOperationNode, but got {type(node)}"

        assert isinstance(
            node.Operator, Token
        ), f"Operation operator should be Token, but got {type(node.Operator)}"
        assert (
            node.Operator.Value == self.operator
        ), f"Operator should be {self.operator}, but got {node.Operator.Value}"

        if self.error is not None:
            assert (
                node.Error == self.error
            ), f"Error should be {self.error}, but got {node.Error}"

        self.exp.Assert(node.Operand)


class ExpressBlockionAsserion(DelayAssertion):
    def __init__(self, *assertions: DelayAssertion) -> None:
        self.assertions = assertions

    def Assert(self, node: Node) -> None:
        assert isinstance(
            node, BlockTypeNode
        ), f"Node should be BlockTypeNode, but got {type(node)}"
        assert (
            node.Type.value == NodeType.EXPRESSION.value
        ), f"Node should be BlockTypeNode, but got {node.Type.name}"
        assert len(node.nodes) == len(
            self.assertions
        ), f"Block should have {len(self.assertions)} nodes, but got {len(node.nodes)}"
        for assertionIndex, assertion in enumerate(self.assertions):
            assertion.Assert(node.nodes[assertionIndex])


class ProgramAssertion(DelayAssertion):
    def __init__(self, *assertions: DelayAssertion) -> None:
        self.assertions = assertions

    def Assert(self, node: Node) -> None:
        assert isinstance(
            node, BlockTypeNode
        ), f"Node should be BlockTypeNode, but got {type(node)}"
        assert (
            node.Type == NodeType.PROGRAM
        ), f"Node should be ProgramNode, but got {node.Type.name}"
        assert len(node.nodes) == len(
            self.assertions
        ), f"Program should have {len(self.assertions)} nodes, but got {len(node.nodes)}"
        for assertionIndex, assertion in enumerate(self.assertions):
            assertion.Assert(node.nodes[assertionIndex])
