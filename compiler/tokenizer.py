from abc import ABC, abstractmethod
from typing import TypeAlias
from enum import Enum, auto
import typing


class TokenType(Enum):
    INTEGER = auto()
    FLOAT = auto()
    STRING = auto()
    NONE = auto()


TokenValue: TypeAlias = typing.Union[int, float, str, None, "Token"]


class Token(ABC):
    @property
    @abstractmethod
    def Type(self) -> TokenType:
        pass

    @property
    @abstractmethod
    def Value(self) -> TokenValue:
        pass


class IntegerToken(Token):
    def __init__(self) -> None:
        super().__init__()

    @property
    def Type(self) -> TokenType:
        return TokenType.INTEGER

    @property
    def Value(self) -> TokenValue:
        return 42


class Tokenizer:
    def __init__(self, content: str) -> None:
        self._content = content

        self._Parse()

    def _Parse(self) -> None:
        pass

    def Next(self) -> Token:
        return IntegerToken()
