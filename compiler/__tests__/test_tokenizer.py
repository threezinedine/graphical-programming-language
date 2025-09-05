import pytest  # type: ignore
from tokenizer import Tokenizer
from utils.tokenizer_assertion import (
    assert_integer_token,
    assert_float_token,
    assert_delimiter_token,
    assert_operator_token,
    assert_string_token,
    assert_invalid_token,
    assert_parentheses_token,
    assert_keyword_token,
    assert_identifier_token,
)


def test_parse_number_as_number():
    assert_integer_token(Tokenizer("42").Next(), 42)
    assert_integer_token(Tokenizer("-42").Next(), -42)
    assert_integer_token(Tokenizer("0").Next(), 0)


def test_parse_float_number():
    assert_float_token(Tokenizer("42.").Next(), 42.0)
    assert_float_token(Tokenizer("-42.").Next(), -42.0)
    assert_float_token(Tokenizer("42.0").Next(), 42.0)
    assert_float_token(Tokenizer("43.1").Next(), 43.1)
    assert_float_token(Tokenizer("42.0").Next(), 42.0)
    assert_float_token(Tokenizer("0.23").Next(), 0.23)
    assert_float_token(Tokenizer(".23").Next(), 0.23)
    assert_float_token(Tokenizer("12.3").Next(), 12.3)


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
    assert_string_token(tokenizer.Next(), '"hello world"')


def test_assign_operator():
    assert_operator_token(Tokenizer("=").Next(), "=")


def test_function_call():
    tokenizer = Tokenizer('print("hello world")')
    assert_identifier_token(tokenizer.Next(), "print")
    assert_parentheses_token(tokenizer.Next(), "(")
    assert_string_token(tokenizer.Next(), '"hello world"')
    assert_parentheses_token(tokenizer.Next(), ")")


def test_parse_multiple_strings():
    tokenizer = Tokenizer('"hello" "world"')
    assert_string_token(tokenizer.Next(), '"hello"')
    assert_string_token(tokenizer.Next(), '"world"')


def test_parse_index_token():
    tokenizer = Tokenizer("array[0]")
    assert_identifier_token(tokenizer.Next(), "array")
    assert_parentheses_token(tokenizer.Next(), "[")
    assert_integer_token(tokenizer.Next(), 0)
    assert_parentheses_token(tokenizer.Next(), "]")


def test_parse_string_with_internal_quotes():
    tokenizer = Tokenizer('"hello \\"world\\""')
    assert_string_token(tokenizer.Next(), '"hello \\"world\\""')


def test_parse_invalid_token():
    assert_invalid_token(Tokenizer("!adn").Next(), "!adn")
    assert_invalid_token(Tokenizer("12adn").Next(), "12adn")


def test_parse_invalid_token_with_other_token_type():
    tokenizer = Tokenizer("23, !adn 42.0")
    assert_integer_token(tokenizer.Next(), 23)
    assert_delimiter_token(tokenizer.Next(), ",")
    assert_invalid_token(tokenizer.Next(), "!adn")
    assert_float_token(tokenizer.Next(), 42.0)


def test_parse_parentheses():
    tokenizer = Tokenizer("(42)")
    assert_parentheses_token(tokenizer.Next(), "(")
    assert_integer_token(tokenizer.Next(), 42)
    assert_parentheses_token(tokenizer.Next(), ")")


def test_parse_multiple_keywords():
    tokenizer = Tokenizer("func testingFunc(a int, b float) void { return; }")
    assert_keyword_token(tokenizer.Next(), "func")
    assert_identifier_token(tokenizer.Next(), "testingFunc")
    assert_parentheses_token(tokenizer.Next(), "(")
    assert_identifier_token(tokenizer.Next(), "a")
    assert_keyword_token(tokenizer.Next(), "int")
    assert_delimiter_token(tokenizer.Next(), ",")
    assert_identifier_token(tokenizer.Next(), "b")
    assert_keyword_token(tokenizer.Next(), "float")
    assert_parentheses_token(tokenizer.Next(), ")")
    assert_keyword_token(tokenizer.Next(), "void")
    assert_parentheses_token(tokenizer.Next(), "{")
    assert_keyword_token(tokenizer.Next(), "return")
    assert_delimiter_token(tokenizer.Next(), ";")
    assert_parentheses_token(tokenizer.Next(), "}")
