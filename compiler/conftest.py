import os
import sys
from typing import Any
import pytest
from parser import Parser


def pytest_configure():
    sys.path.append(os.path.dirname(os.path.abspath(__file__)))
    sys.path.append(
        os.path.join(os.path.dirname(os.path.abspath(__file__)), "__tests__")
    )


class ASTBuilder:
    def __init__(self, content: str) -> None:
        self._content = content

    def Build(self) -> Parser:
        parser = Parser(self._content)
        parser.Program.Compress()
        parser.Program.Parse()
        return parser


@pytest.fixture
def astBuilder() -> Any:
    return ASTBuilder
