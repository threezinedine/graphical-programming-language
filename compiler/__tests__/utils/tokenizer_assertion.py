from tokenizer import TokenType, Token


def assert_integer_token(token: Token, value: int) -> None:
    assert (
        token.Type == TokenType.INTEGER
    ), f"The token must be TokenType.INTEGER, but got {token.Type}"
    assert (
        token.Value == value
    ), f"The token value must be {value}, but got {token.Value}"


def assert_float_token(token: Token, value: float) -> None:
    assert (
        token.Type == TokenType.FLOAT
    ), f"The token must be TokenType.FLOAT, but got {token.Type}"
    assert (
        token.Value == value
    ), f"The token value must be {value}, but got {token.Value}"


def assert_delimiter_token(token: Token, value: str) -> None:
    assert value in [
        ";",
        ",",
    ], f"The delimiter token must be either ';' or ',', but got {value}"

    assert (
        token.Type == TokenType.DELIMITER
    ), f"The token must be TokenType.DELIMITER, but got {token.Type}"
    assert (
        token.Value == value
    ), f"The token value must be {value}, but got {token.Value}"


def assert_operator_token(token: Token, value: str) -> None:
    assert (
        token.Type == TokenType.OPERATOR
    ), f"The token must be TokenType.OPERATOR, but got {token.Type}"
    assert (
        token.Value == value
    ), f'The token value must be "{value}", but got "{token.Value}"'


def assert_string_token(token: Token, value: str) -> None:
    assert (
        token.Type == TokenType.STRING
    ), f"The token must be TokenType.STRING, but got {token.Type}"
    assert (
        token.Value == value
    ), f'The token value must be "{value}", but got "{token.Value}"'


def assert_boolean_token(token: Token, value: str) -> None:
    assert value in [
        "true",
        "false",
    ], f'The boolean token must be either "true" or "false", but got {value}'

    assert (
        token.Type == TokenType.BOOLEAN
    ), f"The token must be TokenType.BOOLEAN, but got {token.Type}"
    assert (
        token.Value == value
    ), f'The token value must be "{value}", but got "{token.Value}"'


def assert_invalid_token(token: Token, value: str) -> None:
    assert (
        token.Type == TokenType.INVALID
    ), f"The token must be TokenType.INVALID, but got {token.Type}"

    assert (
        token.Value == value
    ), f'The token value must be "{value}", but got "{token.Value}"'


def assert_parentheses_token(token: Token, value: str) -> None:
    assert (
        token.Type == TokenType.PARENTHESES
    ), f"The token must be TokenType.PARENTHESES, but got {token.Type}"
    assert (
        token.Value == value
    ), f'The token value must be "{value}", but got "{token.Value}"'


def assert_keyword_token(token: Token, value: str) -> None:
    assert (
        token.Type == TokenType.KEYWORD
    ), f"The token must be TokenType.KEYWORD, but got {token.Type}"
    assert (
        token.Value == value
    ), f'The token value must be "{value}", but got "{token.Value}"'


def assert_identifier_token(token: Token, value: str) -> None:
    assert (
        token.Type == TokenType.IDENTIFIER
    ), f"The token must be TokenType.IDENTIFIER, but got {token.Type}"
    assert (
        token.Value == value
    ), f'The token value must be "{value}", but got "{token.Value}"'
