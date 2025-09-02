import pytest  # type: ignore
from tokenizer import TokenType, Tokenizer


def test_parse_number_as_number():
    tokenizer = Tokenizer("42")
    token = tokenizer.Next()

    assert token.Type == TokenType.INTEGER
    assert token.Value == 42
