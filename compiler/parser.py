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

    INVALID_EXPRESSION = auto()
    INVALID_BLOCK = auto()
    INVALID_INDEX_BLOCK = auto()
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
    def ToJson(self) -> Dict[str, Any]:
        pass

    def __repr__(self) -> str:
        return json.dumps(self.ToJson(), indent=4)


class InvalidNode(ABC):
    @property
    @abstractmethod
    def Error(self) -> str:
        pass


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


class BlockTypeNode(Node):
    def __init__(self, nodeType: NodeType, nodes: list[Node]) -> None:
        self.nodes = nodes
        self.type = nodeType

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
        }

    def Compress(self) -> None:
        self.nodes = self._CompressParenthesis(
            self.nodes,
            "(",
            NodeType.EXPRESSION,
            NodeType.INVALID_EXPRESSION,
        )
        self.nodes = self._CompressParenthesis(
            self.nodes,
            "{",
            NodeType.BLOCK,
            NodeType.INVALID_BLOCK,
        )
        self.nodes = self._CompressParenthesis(
            self.nodes,
            "[",
            NodeType.INDEX_BLOCK,
            NodeType.INVALID_INDEX_BLOCK,
        )

    def _CompressParenthesis(
        self,
        nodes: list[Node],
        parenthesis: str,
        validGroup: NodeType,
        invalidGroup: NodeType,
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
                            invalidGroup,
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
            newNode = InvalidBlockNode(
                invalidGroup,
                self._CompressParenthesis(
                    deepcopy(currentTempsBlock),
                    parenthesis,
                    validGroup,
                    invalidGroup,
                ),
                f'Lack of closing parenthesis "{parenthesis}"',
            )
            newNode.Compress()
            result.append(newNode)

        return result


class InvalidBlockNode(BlockTypeNode, InvalidNode):
    def __init__(self, blockType: NodeType, nodes: list["Node"], error: str) -> None:
        super().__init__(blockType, nodes)
        self.error = error

    @property
    def Error(self) -> str:
        return self.error

    def __repr__(self) -> str:
        return f"{self.__class__.__name__}(Error: {self.error})"


class Parser:
    def __init__(self, content: str) -> None:
        self._content = content
        self._tokenizer = Tokenizer(content)

        self._programNode: BlockTypeNode = BlockTypeNode(
            NodeType.PROGRAM,
            [AtomicNode(token=token) for token in self._tokenizer.Tokens],
        )

        self._programNode.Compress()

    @property
    def Program(self) -> BlockTypeNode:
        return self._programNode
