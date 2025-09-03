from abc import ABC
import typing
from tokenizer import Tokenizer, Token, TokenType, TYPES


class Node(ABC):
    pass


class FunctionCall(Node):
    def __init__(self, functionName: str, parameters: list["Parameter"]) -> None:
        self.functionName = functionName
        self.parameters = parameters


class VariableDefinition(Node):
    def __init__(
        self, identifier: str, varType: str, initialValue: Node | None = None
    ) -> None:
        self.identifier = identifier
        self.varType = varType
        self.initialValue = initialValue


class Parameter(Node):
    def __init__(self, exp: Node) -> None:
        self.exp = exp


class String(Node):
    def __init__(self, value: str) -> None:
        super().__init__()
        self.value = value


class Integer(Node):
    def __init__(self, value: int) -> None:
        super().__init__()
        self.value = value


class Float(Node):
    def __init__(self, value: float) -> None:
        super().__init__()
        self.value = value


class Parser:
    def __init__(self, content: str) -> None:
        self._content = content
        self._tokenizer = Tokenizer(content)

        self._nodes: list[Node] = []

        self._processes: list[typing.Callable[[list[Token]], Node | None]] = [
            self._ProcessInteger,
            self._ProcessFloat,
            self._ProcessStringLiteral,
            self._ProcessVariableDefinition,
            self._ProcessFunctionCall,
        ]

        self._Parse()

    def _Parse(self) -> None:
        tempTokens: list[Token] = []
        while not self._tokenizer.Empty():
            token = self._tokenizer.Next()
            if token.Type != TokenType.DELIMITER or token.Value != ";":
                tempTokens.append(token)
                continue

            if len(tempTokens) == 0:
                tempTokens = []
                continue

            for process in self._processes:
                result = process(tempTokens)
                if result:
                    self._nodes.append(result)
                    break

            tempTokens = []

    def _ProcessStringLiteral(self, tokens: list[Token]) -> Node | None:
        if len(tokens) != 1:
            return None

        if tokens[0].Type != TokenType.STRING:
            return None

        assert isinstance(tokens[0].Value, str)
        return String(tokens[0].Value)

    def _ProcessInteger(self, token: list[Token]) -> Node | None:
        if len(token) != 1:
            return None

        if token[0].Type != TokenType.INTEGER:
            return None

        assert isinstance(token[0].Value, int)
        return Integer(token[0].Value)

    def _ProcessFloat(self, token: list[Token]) -> Node | None:
        if len(token) != 1:
            return None

        if token[0].Type != TokenType.FLOAT:
            return None

        assert isinstance(token[0].Value, float)
        return Float(token[0].Value)

    def _ProcessVariableDefinition(self, tokens: list[Token]) -> Node | None:
        if len(tokens) < 4:
            return

        if tokens[0].Type != TokenType.KEYWORD or (
            tokens[0].Value not in TYPES and tokens[0].Value != "auto"
        ):
            return

        if tokens[1].Type != TokenType.IDENTIFIER:
            return

        if tokens[2].Type != TokenType.OPERATOR or tokens[2].Value != "=":
            return

        node: Node | None = None

        for process in self._processes:
            node = process(tokens[3:])
            if node:
                break

        assert isinstance(tokens[0].Value, str)
        assert isinstance(tokens[1].Value, str)
        return VariableDefinition(tokens[1].Value, tokens[0].Value, node)

    def _ProcessFunctionCall(self, tokens: list[Token]) -> Node | None:
        if len(tokens) < 3:
            return None

        if tokens[0].Type != TokenType.IDENTIFIER:
            return None

        if tokens[1].Type != TokenType.PARENTHESES or tokens[1].Value != "(":
            return None

        if tokens[-1].Type != TokenType.PARENTHESES or tokens[-1].Value != ")":
            return None

        functionName: str = tokens[0].Value  # type: ignore
        parameters: list[Parameter] = []

        tempTokens: list[Token] = []
        for i in range(2, len(tokens) - 1):
            if tokens[i].Type != TokenType.DELIMITER or tokens[i].Value != ",":
                tempTokens.append(tokens[i])

                if i != len(tokens) - 2:
                    continue

            if len(tempTokens) == 0:
                continue

            for process in self._processes:
                result = process(tempTokens)
                if result:
                    parameters.append(Parameter(result))
                    break

            tempTokens = []

        return FunctionCall(functionName, parameters)

    @property
    def Nodes(self) -> list[Node]:
        return self._nodes
