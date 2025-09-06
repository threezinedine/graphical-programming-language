from abc import ABC, abstractmethod
from copy import deepcopy
from enum import Enum, auto
import json
from typing import Any, Dict, TypeAlias, Union

from tokenizer import (
    MATCH_CLOSE_PARENTHESIS,
    Tokenizer,
    Token,
    TokenType,
)

HierarchyToken: TypeAlias = Union[Token, list["HierarchyToken"]]


class NodeType(Enum):
    ATOMIC = auto()
    PROGRAM = auto()
    EXPRESSION = auto()
    BLOCK = auto()
    INDEX_BLOCK = auto()
    OPERATION_EXPRESSION = auto()
    UNARY_OPERATION_EXPRESSION = auto()

    IF_STATEMENT = auto()
    STATEMENT = auto()

    INVALID = auto()


class Node(ABC):
    @property
    @abstractmethod
    def Type(self) -> NodeType:
        pass

    @abstractmethod
    def __len__(self) -> int:
        pass

    @abstractmethod
    def __getitem__(self, index: int) -> "Node":
        pass

    @abstractmethod
    def Compress(self) -> None:
        pass

    @abstractmethod
    def Parse(self) -> None:
        pass

    @abstractmethod
    def ToJson(self) -> Dict[str, Any]:
        pass

    def __repr__(self) -> str:
        return json.dumps(self.ToJson(), indent=4)

    @property
    @abstractmethod
    def IsError(self) -> bool:
        pass

    @property
    def Error(self) -> str:
        if self.IsError:
            assert isinstance(self, InvalidNode)
            return self.Error
        return ""


class InvalidNode(Node):
    @property
    def Type(self) -> NodeType:
        return NodeType.INVALID

    def __len__(self) -> int:
        return 0

    def __getitem__(self, index: int) -> "Node":
        raise IndexError("Invalid node has no children")

    def ToJson(self) -> Dict[str, Any]:
        return {
            "type": self.Type.name,
            "error": "Invalid node",
        }

    def Compress(self) -> None:
        pass

    def Parse(self) -> None:
        pass

    @property
    def IsError(self) -> bool:
        return True

    @property
    def Error(self) -> str:
        return "Invalid node"


class AtomicNode(Node):
    def __init__(self, token: Token) -> None:
        super().__init__()
        self.token = token

    @property
    def Type(self) -> NodeType:
        return NodeType.ATOMIC

    def __len__(self) -> int:
        return 1

    def __getitem__(self, index: int) -> "Node":
        assert index == 0, "Index out of range"
        return self

    def ToJson(self) -> Dict[str, Any]:
        return {
            "type": self.Type.name,
            "value": self.token.Value,
            "tokenType": self.token.Type.name,
        }

    def Compress(self) -> None:
        pass

    def Parse(self) -> None:
        pass

    @property
    def IsError(self) -> bool:
        return self.token.Type == TokenType.INVALID

    @property
    def Error(self) -> str:
        if self.IsError:
            return f"Invalid token: {self.token.Value}"
        return ""


class BlockTypeNode(Node):
    def __init__(
        self, nodeType: NodeType, nodes: list[Node], error: str | None = None
    ) -> None:
        self.nodes = nodes
        self.type = nodeType
        self.error = error

    def __len__(self) -> int:
        return len(self.nodes)

    @property
    def Type(self) -> NodeType:
        return self.type

    def __getitem__(self, index: int) -> Node:
        return self.nodes[index]

    def ToJson(self) -> Dict[str, Any]:
        return {
            "type": self.Type.name,
            "nodes": [node.ToJson() for node in self.nodes],
            "error": self.error,
        }

    @property
    def IsError(self) -> bool:
        return self.error is not None

    @property
    def Error(self) -> str:
        if self.IsError:
            assert self.error is not None
            return self.error
        return ""

    def Parse(self) -> None:
        for node in self.nodes:
            if node.Type in [
                NodeType.BLOCK,
                NodeType.EXPRESSION,
            ]:
                node.Parse()

        while not self._ParseUnaryOperation("|", "!"):
            pass

        while not self._ParseOperation("^"):
            pass

        while not self._ParseOperation("*", "/"):
            pass

        while not self._ParseOperation("+", "-"):
            pass

        while not self._ParseOperation("<", "<=", ">", ">=", "==", "!="):
            pass

        while not self._ParseOperation("+=", "-=", "*=", "/=", "%="):
            pass

        while not self._ParseOperation("="):
            pass

        if self.Type in [
            NodeType.PROGRAM,
            NodeType.BLOCK,
        ]:
            while not self._ParseIfStatement():
                pass

            while not self._ParseStatement():
                pass

    def _ParseIfStatement(self) -> bool:
        tempNodes: list[Node] = []

        nodeIndex = 0

        while nodeIndex < len(self.nodes):
            keywordNode = self.nodes[nodeIndex]

            if not (
                keywordNode.Type == NodeType.ATOMIC
                and isinstance(keywordNode, AtomicNode)
                and keywordNode.token.Type == TokenType.KEYWORD
                and keywordNode.token.Value == "if"
            ):
                tempNodes.append(deepcopy(keywordNode))
                nodeIndex += 1
                continue

            indexAddition = 1

            conditionNode: BlockTypeNode | InvalidNode = InvalidNode()
            bodyNode: BlockTypeNode
            error: str | None = None
            conditionNodeIndex = 1
            blockNodeIndex = 2

            if (
                nodeIndex + conditionNodeIndex >= len(self.nodes)
                or self.nodes[nodeIndex + conditionNodeIndex].Type
                != NodeType.EXPRESSION
            ):
                conditionNode = InvalidNode()
                error = "Missing condition expression"

                if nodeIndex + conditionNodeIndex < len(self.nodes):
                    blockNodeIndex = conditionNodeIndex
            else:
                conditionNode = self.nodes[nodeIndex + conditionNodeIndex]  # type: ignore
                indexAddition += 1

            if (
                nodeIndex + blockNodeIndex >= len(self.nodes)
                or self.nodes[nodeIndex + blockNodeIndex].Type != NodeType.BLOCK
            ):
                bodyNode = BlockTypeNode(NodeType.BLOCK, [])
                error = "Missing body block"
            else:
                bodyNode = self.nodes[nodeIndex + blockNodeIndex]  # type: ignore
                indexAddition += 1

            newIfStatementNode = IfStatementNode(
                condition=conditionNode,
                body=bodyNode,
                error=error,
            )

            tempNodes.append(newIfStatementNode)
            nodeIndex += indexAddition

        self.nodes = deepcopy(tempNodes)

        return True

    def _ParseStatement(self) -> bool:
        tempNodes: list[Node] = []

        tempStatmentNodes: list[Node] = []
        nodeIndex = 0
        while nodeIndex < len(self.nodes):
            currentNode = self.nodes[nodeIndex]

            if currentNode.Type in [
                NodeType.BLOCK,
                NodeType.IF_STATEMENT,
                NodeType.STATEMENT,
            ]:
                if len(tempStatmentNodes) > 0:
                    newStatementNode = BlockTypeNode(
                        NodeType.STATEMENT,
                        deepcopy(tempStatmentNodes),
                        error="Missing semicolon at the end",
                    )
                    tempNodes.append(newStatementNode)

                tempNodes.append(deepcopy(currentNode))
                tempStatmentNodes = []
                nodeIndex += 1
                continue

            if not (
                currentNode.Type == NodeType.ATOMIC
                and isinstance(currentNode, AtomicNode)
                and currentNode.token.Type == TokenType.DELIMITER
                and currentNode.token.Value == ";"
            ):
                tempStatmentNodes.append(deepcopy(currentNode))
                nodeIndex += 1
                continue

            newStatementNode = BlockTypeNode(
                NodeType.STATEMENT,
                deepcopy(tempStatmentNodes),
            )
            tempNodes.append(newStatementNode)
            tempStatmentNodes = []

            nodeIndex += 1

        self.nodes = deepcopy(tempNodes)

        if len(tempStatmentNodes) > 0:
            newStatementNode = BlockTypeNode(
                NodeType.STATEMENT,
                deepcopy(tempStatmentNodes),
                error="Missing semicolon at the end",
            )
            self.nodes.append(newStatementNode)

        return True

    def _ParseUnaryOperation(self, *operations: str) -> bool:
        hasAnyChange = False
        tempNodes: list[Node] = []

        nodeIndex = 0
        while nodeIndex < len(self.nodes):
            operatorNode = self.nodes[nodeIndex]

            if not (
                operatorNode.Type == NodeType.ATOMIC
                and isinstance(operatorNode, AtomicNode)
                and operatorNode.token.Type == TokenType.OPERATOR
                and operatorNode.token.Value in operations
            ):
                tempNodes.append(deepcopy(operatorNode))
                nodeIndex += 1
                continue

            def checkOperandNodeValid(node: Node) -> bool:
                return (
                    (
                        isinstance(node, AtomicNode)
                        and node.token.Type
                        in [
                            TokenType.INTEGER,
                            TokenType.FLOAT,
                            TokenType.IDENTIFIER,
                            TokenType.STRING,
                        ]
                    )
                    or (
                        isinstance(node, BlockTypeNode)
                        and node.Type == NodeType.EXPRESSION
                    )
                    or isinstance(node, OperationNode)
                    or isinstance(node, UnaryOperationNode)
                )

            operandNode: Node | None = None

            if nodeIndex + 1 >= len(self.nodes):
                operandNode = None
            else:
                operandNode = self.nodes[nodeIndex + 1]
                operandNode.Parse()

            if operandNode and not checkOperandNodeValid(operandNode):
                operandNode = None

            error: str | None = None

            if operandNode is None and operandNode is None:
                error = f"Operand of operator '{operatorNode.token.Value}' is invalid"

            newOperationNode = UnaryOperationNode(
                operatorNode.token,
                (deepcopy(operandNode) if operandNode is not None else InvalidNode()),
                error,
            )
            if operandNode is None:
                nodeIndex += 1
            else:
                nodeIndex += 2
            tempNodes.append(newOperationNode)
            hasAnyChange = True

        if hasAnyChange:
            self.nodes = deepcopy(tempNodes)
        return not hasAnyChange

    def _ParseOperation(self, *operations: str) -> bool:
        hasAnyChange = False
        tempNodes: list[Node] = []

        nodeIndex = 0
        while nodeIndex < len(self.nodes):
            operatorNode = self.nodes[nodeIndex]

            if not (
                operatorNode.Type == NodeType.ATOMIC
                and isinstance(operatorNode, AtomicNode)
                and operatorNode.token.Type == TokenType.OPERATOR
                and operatorNode.token.Value in operations
            ):
                tempNodes.append(deepcopy(operatorNode))
                nodeIndex += 1
                continue

            def checkOperandNodeValid(node: Node) -> bool:
                return (
                    (
                        isinstance(node, AtomicNode)
                        and node.token.Type
                        in [
                            TokenType.INTEGER,
                            TokenType.FLOAT,
                            TokenType.IDENTIFIER,
                            TokenType.STRING,
                        ]
                    )
                    or (
                        isinstance(node, BlockTypeNode)
                        and node.Type == NodeType.EXPRESSION
                    )
                    or isinstance(node, OperationNode)
                    or isinstance(node, UnaryOperationNode)
                )

            operandLeftNode: Node | None = None
            operandRightNode: Node | None = None

            if len(tempNodes) == 0:
                operandLeftNode = None
            else:
                operandLeftNode = tempNodes[-1]
                operandLeftNode.Parse()

            if nodeIndex + 1 >= len(self.nodes):
                operandRightNode = None
            else:
                operandRightNode = self.nodes[nodeIndex + 1]
                operandRightNode.Parse()

            if operandLeftNode and not checkOperandNodeValid(operandLeftNode):
                operandLeftNode = None

            if operandRightNode is not None and not checkOperandNodeValid(
                operandRightNode
            ):
                operandRightNode = None

            error: str | None = None

            if operandRightNode is None and operandLeftNode is None:
                error = (
                    f"Both sides of operator '{operatorNode.token.Value}' are invalid"
                )
            elif operandRightNode is None:
                error = (
                    f"Right side of operator '{operatorNode.token.Value}' is invalid"
                )
            elif operandLeftNode is None:
                error = f"Left side of operator '{operatorNode.token.Value}' is invalid"

            newOperationNode = OperationNode(
                operatorNode.token,
                (
                    deepcopy(operandLeftNode)
                    if operandLeftNode is not None
                    else InvalidNode()
                ),
                (
                    deepcopy(operandRightNode)
                    if operandRightNode is not None
                    else InvalidNode()
                ),
                error,
            )
            if operandLeftNode is None and operandRightNode is None:
                nodeIndex += 1
            elif operandLeftNode is None:
                nodeIndex += 2
            elif operandRightNode is None:
                nodeIndex += 1
                tempNodes.pop()  # Remove the left operand
            else:
                nodeIndex += 2
                tempNodes.pop()  # Remove the left operand
            tempNodes.append(newOperationNode)
            hasAnyChange = True

        if hasAnyChange:
            self.nodes = deepcopy(tempNodes)
        return not hasAnyChange

    def Compress(self) -> None:
        self.nodes = self._CompressParenthesis(
            self.nodes,
            "(",
            NodeType.EXPRESSION,
        )
        self.nodes = self._CompressParenthesis(
            self.nodes,
            "{",
            NodeType.BLOCK,
        )
        self.nodes = self._CompressParenthesis(
            self.nodes,
            "[",
            NodeType.INDEX_BLOCK,
        )

    def _CompressParenthesis(
        self,
        nodes: list[Node],
        parenthesis: str,
        validGroup: NodeType,
    ) -> list[Node]:
        result: list[Node] = []

        parenthesisStackCount = 0
        currentTempsBlock: list[Node] = []
        inBlock = False

        for node in nodes:
            if not isinstance(node, AtomicNode):
                if inBlock:
                    currentTempsBlock.append(deepcopy(node))
                else:
                    result.append(deepcopy(node))
                continue

            token = node.token

            if token.Type == TokenType.PARENTHESES and token.Value == parenthesis:
                if parenthesisStackCount == 0:
                    inBlock = True
                    currentTempsBlock = []
                else:
                    currentTempsBlock.append(deepcopy(node))
                parenthesisStackCount += 1
            elif (
                token.Type == TokenType.PARENTHESES
                and token.Value == MATCH_CLOSE_PARENTHESIS[parenthesis]
                and inBlock
            ):
                if parenthesisStackCount > 1:
                    currentTempsBlock.append(deepcopy(node))
                else:
                    inBlock = False
                    newNode = BlockTypeNode(
                        validGroup,
                        self._CompressParenthesis(
                            deepcopy(currentTempsBlock),
                            parenthesis,
                            validGroup,
                        ),
                    )
                    newNode.Compress()
                    result.append(newNode)
                    currentTempsBlock = []
                parenthesisStackCount -= 1
            else:
                if inBlock:
                    currentTempsBlock.append(deepcopy(node))
                else:
                    result.append(deepcopy(node))

        if parenthesisStackCount != 0:
            newNode = BlockTypeNode(
                validGroup,
                self._CompressParenthesis(
                    deepcopy(currentTempsBlock),
                    parenthesis,
                    validGroup,
                ),
                f'Lack of closing parenthesis "{parenthesis}"',
            )
            newNode.Compress()
            result.append(newNode)

        return result


class UnaryOperationNode(Node):
    def __init__(
        self, operator: Token, operand: Node, error: str | None = None
    ) -> None:
        self.operator = operator
        self.operand = operand
        self.error = error

    @property
    def Type(self) -> NodeType:
        return NodeType.UNARY_OPERATION_EXPRESSION

    def __len__(self) -> int:
        return 2

    def __getitem__(self, index: int) -> Node:
        if index == 0:
            return AtomicNode(self.operator)
        elif index == 1:
            return self.operand
        else:
            raise IndexError("Index out of range")

    @property
    def IsError(self) -> bool:
        return self.error is not None

    @property
    def Error(self) -> str:
        if self.IsError:
            assert self.error is not None
            return self.error
        return ""

    @property
    def Operator(self) -> Token:
        return self.operator

    @property
    def Operand(self) -> Node:
        return self.operand

    def ToJson(self) -> Dict[str, Any]:
        return {
            "type": self.Type.name,
            "operator": {
                "value": self.operator.Value,
                "tokenType": self.operator.Type.name,
            },
            "operand": self.operand.ToJson(),
            "error": self.error,
        }

    def Compress(self) -> None:
        self.operand.Compress()

    def Parse(self) -> None:
        self.operand.Parse()


class OperationNode(Node):
    def __init__(
        self, operator: Token, left: Node, right: Node, error: str | None = None
    ) -> None:
        self.operator = operator
        self.left = left
        self.right = right
        self.error = error

    @property
    def Type(self) -> NodeType:
        return NodeType.OPERATION_EXPRESSION

    def __len__(self) -> int:
        return 3

    def __getitem__(self, index: int) -> Node:
        if index == 0:
            return self.left
        elif index == 1:
            return AtomicNode(self.operator)
        elif index == 2:
            return self.right
        else:
            raise IndexError("Index out of range")

    @property
    def Left(self) -> Node:
        return self.left

    @property
    def Operator(self) -> Token:
        return self.operator

    @property
    def Right(self) -> Node:
        return self.right

    @property
    def IsError(self) -> bool:
        return self.error is not None

    @property
    def Error(self) -> str:
        if self.IsError:
            assert self.error is not None
            return self.error
        return ""

    def ToJson(self) -> Dict[str, Any]:
        return {
            "type": self.Type.name,
            "operator": {
                "value": self.operator.Value,
                "tokenType": self.operator.Type.name,
            },
            "left": self.left.ToJson(),
            "right": self.right.ToJson(),
            "error": self.error,
        }

    def Compress(self) -> None:
        self.left.Compress()
        self.right.Compress()

    def Parse(self) -> None:
        self.left.Parse()
        self.right.Parse()


class IfStatementNode(Node):
    def __init__(
        self,
        condition: Node,
        body: Node,
        error: str | None = None,
    ) -> None:
        super().__init__()
        self.condition = condition
        self.body = body
        self.error = error

    @property
    def Type(self) -> NodeType:
        return NodeType.IF_STATEMENT

    @property
    def IsError(self) -> bool:
        return self.error is not None

    @property
    def Error(self) -> str:
        if self.IsError:
            assert self.error is not None
            return self.error
        return ""

    def ToJson(self) -> Dict[str, Any]:
        return {
            "type": self.Type.name,
            "condition": self.condition.ToJson(),
            "body": self.body.ToJson(),
            "error": self.error,
        }

    def __len__(self) -> int:
        return 2

    def __getitem__(self, index: int) -> Node:
        if index == 0:
            return self.condition
        elif index == 1:
            return self.body
        else:
            raise IndexError("Index out of range")

    @property
    def Condition(self) -> Node:
        return self.condition

    @property
    def BodyBlock(self) -> Node:
        return self.body

    def Compress(self) -> None:
        self.condition.Compress()
        self.body.Compress()

    def Parse(self) -> None:
        self.condition.Parse()
        self.body.Parse()


class Parser:
    def __init__(self, content: str) -> None:
        self._content = content
        self._tokenizer = Tokenizer(content)

        self._programNode: BlockTypeNode = BlockTypeNode(
            NodeType.PROGRAM,
            [AtomicNode(token=token) for token in self._tokenizer.Tokens],
        )

    @property
    def Program(self) -> BlockTypeNode:
        return self._programNode
