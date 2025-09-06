from typing import TypeAlias
from enum import Enum, auto
import typing
import re


class TokenType(Enum):
    BOOLEAN = auto()
    INTEGER = auto()
    FLOAT = auto()
    STRING = auto()
    DELIMITER = auto()
    OPERATOR = auto()
    PARENTHESES = auto()
    IDENTIFIER = auto()
    KEYWORD = auto()
    NONE = auto()
    INVALID = auto()


MATCH_CLOSE_PARENTHESIS = {
    "(": ")",
    "{": "}",
    "[": "]",
}

OpenParenthesis: TypeAlias = typing.Union[
    typing.Literal["("], typing.Literal["{"], typing.Literal["["]
]
TokenValue: TypeAlias = typing.Union[int, float, str, None, "Token"]

TYPES = [
    "int",
    "float",
    "string",
    "void",
]

KEYWORDS = [
    "func",
    "auto",
    "return",
    "if",
    # type keywords
    *TYPES,
]
KEYWORD_REGEX = "|".join(KEYWORDS)

OPERATORS = [
    "\\+=",
    "\\-=",
    "\\*=",
    "\\/=",
    "\\%=",
    "\\^=",
    "==",
    "!=",
    "\\|",
    "<",
    "<=",
    ">",
    ">=",
]
OPERATORS_REGEX = "|".join([rf"({op})" for op in OPERATORS])


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

    def __repr__(self) -> str:
        return f"Token(Type: {self._type}, Value: {self._value})"


class Tokenizer:
    REGEXES: dict[TokenType, list[str]] = {
        TokenType.KEYWORD: [rf"^({KEYWORD_REGEX})"],
        TokenType.DELIMITER: [r"^[,;]"],
        TokenType.PARENTHESES: [r"^[(){}\[\]]"],
        TokenType.INVALID: [r"^\d+[a-zA-Z]+"],
        TokenType.OPERATOR: [
            rf"^({OPERATORS_REGEX})",
            r"^!",
            r"^=",
            r"^[\\+\\*\\/\\%\\^]",
            r"^-",
        ],
        TokenType.BOOLEAN: [r"^(true|false)"],
        TokenType.FLOAT: [r"^\d*\.\d*"],
        TokenType.INTEGER: [r"^\d+"],
        TokenType.STRING: [r'^"((?:[^"\\]|\\.)*)"'],
        TokenType.IDENTIFIER: [r"^[a-zA-Z_][a-zA-Z0-9_]*"],
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
            if len(tempContent) == 0:
                break

            while (len(tempContent) > 0 and tempContent[0] == " ") or (
                len(tempContent) > 0 and tempContent[0] == "\n"
            ):
                tempContent = tempContent[1:]

            if len(tempContent) == 0:
                break

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
                        self._tokens.append(Token(TokenType.STRING, match.group(0)))
                    elif tokenType == TokenType.PARENTHESES:
                        self._tokens.append(
                            Token(TokenType.PARENTHESES, match.group(0))
                        )
                    elif tokenType == TokenType.KEYWORD:
                        self._tokens.append(Token(TokenType.KEYWORD, match.group(0)))
                    elif tokenType == TokenType.IDENTIFIER:
                        self._tokens.append(Token(TokenType.IDENTIFIER, match.group(0)))
                    elif tokenType == TokenType.INVALID:
                        self._tokens.append(Token(TokenType.INVALID, match.group(0)))
                    elif tokenType == TokenType.OPERATOR:
                        self._tokens.append(Token(TokenType.OPERATOR, match.group(0)))
                    elif tokenType == TokenType.BOOLEAN:
                        self._tokens.append(Token(TokenType.BOOLEAN, match.group(0)))

                    break

            if not hasMatch:
                nextSpace = tempContent.find(" ")
                nextNewLine = tempContent.find("\n")

                if nextSpace == -1 and nextNewLine == -1:
                    self._tokens.append(Token(TokenType.INVALID, tempContent))
                    break
                if nextNewLine == -1 or (nextSpace != -1 and nextSpace < nextNewLine):
                    self._tokens.append(
                        Token(TokenType.INVALID, tempContent[:nextSpace])
                    )
                    tempContent = tempContent[nextSpace + 1 :]
                else:
                    self._tokens.append(
                        Token(TokenType.INVALID, tempContent[:nextNewLine])
                    )
                    tempContent = tempContent[nextNewLine + 1 :]

    def Empty(self) -> bool:
        return len(self._tokens) == 0

    def Next(self) -> Token:
        return self._tokens.pop(0)

    @property
    def Tokens(self) -> list[Token]:
        return self._tokens
