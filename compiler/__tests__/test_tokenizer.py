import pytest  # type: ignore
from tokenizer import Tokenizer
from utils.tokenizer_assertion import (
    assert_integer_token,
    assert_float_token,
    assert_delimiter_token,
    assert_string_token,
)


def test_parse_number_as_number():
    assert_integer_token(Tokenizer("42").Next(), 42)
    assert_integer_token(Tokenizer("-42").Next(), -42)
    assert_integer_token(Tokenizer("0").Next(), 0)


def test_parse_float_number():
    assert_float_token(Tokenizer("42f").Next(), 42.0)
    assert_float_token(Tokenizer("-42f").Next(), -42.0)
    assert_float_token(Tokenizer("42.0").Next(), 42.0)
    assert_float_token(Tokenizer("43.1f").Next(), 43.1)
    assert_float_token(Tokenizer("42.f0").Next(), 42.0)
    assert_float_token(Tokenizer("0.23").Next(), 0.23)
    assert_float_token(Tokenizer("12.3f1").Next(), 12.3)


def test_parse_multiple_numbers():
    tokenizer = Tokenizer("23 43.2 12")
    assert_integer_token(tokenizer.Next(), 23)
    assert_float_token(tokenizer.Next(), 43.2)
    assert_integer_token(tokenizer.Next(), 12)


def test_parse_delimiter():
    tokenizer = Tokenizer("23,\n 43.2; 12")
    assert_integer_token(tokenizer.Next(), 23)
    assert_delimiter_token(tokenizer.Next(), ",")
    assert_float_token(tokenizer.Next(), 43.2)
    assert_delimiter_token(tokenizer.Next(), ";")
    assert_integer_token(tokenizer.Next(), 12)


def test_parse_string():
    tokenizer = Tokenizer('"hello world"')
    assert_string_token(tokenizer.Next(), "hello world")


def test_parse_multiple_strings():
    tokenizer = Tokenizer('"hello" "world"')
    assert_string_token(tokenizer.Next(), "hello")
    assert_string_token(tokenizer.Next(), "world")


def test_parse_string_with_internal_quotes():
    tokenizer = Tokenizer('"hello \\"world\\""')
    assert_string_token(tokenizer.Next(), 'hello \\"world\\"')
