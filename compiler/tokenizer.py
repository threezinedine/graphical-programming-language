from typing import TypeAlias
from enum import Enum, auto
import typing
import re


class TokenType(Enum):
    INTEGER = auto()
    FLOAT = auto()
    STRING = auto()
    DELIMITER = auto()
    NONE = auto()


TokenValue: TypeAlias = typing.Union[int, float, str, None, "Token"]


class Token:
    def __init__(self, type: TokenType, value: TokenValue) -> None:
        self._type: TokenType = type
        self._value: TokenValue = value

    @property
    def Type(self) -> TokenType:
        return self._type

    @property
    def Value(self) -> TokenValue:
        return self._value


class Tokenizer:
    REGEXES: dict[TokenType, list[str]] = {
        TokenType.DELIMITER: [r"^[,;]"],
        TokenType.FLOAT: [r"^-?\d+f", r"^-?\d+\.\d*f?"],
        TokenType.INTEGER: [r"^-?\d+"],
        TokenType.STRING: [r'^"((?:[^"\\]|\\.)*)"'],
    }

    def __init__(self, content: str) -> None:
        self._content = content
        self._regexes: dict[str, TokenType] = {}
        self._TransformRegexes()

        self._tokens: list[Token] = []

        self._Parse()

    def _TransformRegexes(self) -> None:
        for tokenType, regexes in self.REGEXES.items():
            for regex in regexes:
                self._regexes[regex] = tokenType

    def _Parse(self) -> None:
        tempContent = self._content
        while tempContent:
            hasMatch = False
            for regex, tokenType in self._regexes.items():
                match = re.match(regex, tempContent)
                if match:
                    hasMatch = True
                    tempContent = tempContent[match.end() :]
                    if tokenType == TokenType.INTEGER:
                        self._tokens.append(
                            Token(TokenType.INTEGER, int(match.group(0)))
                        )
                    elif tokenType == TokenType.FLOAT:
                        matchToken = match.group(0)
                        fPosition = matchToken.find("f")
                        finalToken = match.group(0)
                        if fPosition != -1:
                            finalToken = matchToken[:fPosition]
                        self._tokens.append(Token(TokenType.FLOAT, float(finalToken)))
                    elif tokenType == TokenType.DELIMITER:
                        self._tokens.append(Token(TokenType.DELIMITER, match.group(0)))
                    elif tokenType == TokenType.STRING:
                        self._tokens.append(
                            Token(TokenType.STRING, match.group(0)[1:-1])
                        )

                    break

            if not hasMatch:
                nextSpace = tempContent.find(" ")
                nextNewLine = tempContent.find("\n")

                if nextSpace == -1 and nextNewLine == -1:
                    break
                if nextNewLine == -1 or (nextSpace != -1 and nextSpace < nextNewLine):
                    tempContent = tempContent[nextSpace + 1 :]
                else:
                    tempContent = tempContent[nextNewLine + 1 :]

    def Next(self) -> Token:
        return self._tokens.pop(0)
