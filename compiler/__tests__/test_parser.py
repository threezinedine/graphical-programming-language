import pytest  # type: ignore
from utils.parser_assertion import (
    AtomicAssertion,
    InvalidAtomicAssertion,
    OperationAssertion,
    ProgramAssertion,
    ExpressBlockionAsserion,
)
from compiler.tokenizer import TokenType
from parser import Parser


def test_compress_expressions_to_nodes():
    ast = Parser(
        """
3 + (23 - 5)
"""
    )

    ast.Program.Compress()
    assert len(ast.Program) == 3
    assert len(ast.Program[-1]) == 3


def test_compress_nested_parenthesis():
    ast = Parser(
        """
3 - (3 + (23 - 5)) * 2
"""
    )

    ast.Program.Compress()
    assert len(ast.Program) == 5
    assert len(ast.Program[2]) == 3
    assert len(ast.Program[2][2]) == 3


def test_compress_multiple_expressions():
    ast = Parser(
        """
(3 + 5) * (10 - 2)
"""
    )

    ast.Program.Compress()
    assert len(ast.Program) == 3
    assert len(ast.Program[0]) == 3
    assert len(ast.Program[2]) == 3


def test_compress_no_expressions():
    ast = Parser(
        """ 
3 + 5 * 10 - 2
"""
    )

    ast.Program.Compress()
    assert len(ast.Program) == 7


def test_compress_empty_expressions():
    ast = Parser(
        """
        """
    )

    ast.Program.Compress()
    assert len(ast.Program) == 0


def test_compress_single_expression():
    ast = Parser(
        """
(3 + 5)
"""
    )

    ast.Program.Compress()
    assert len(ast.Program) == 1
    assert len(ast.Program[0]) == 3


def test_compress_unmatched_parenthesis():
    ast = Parser(
        """
(3 + 5 * (10 - 2)
"""
    )

    ast.Program.Compress()
    assert len(ast.Program) == 1
    assert len(ast.Program[0]) == 5
    assert len(ast.Program[0][4]) == 3


def test_compress_unmatched_closing_parenthesis():
    ast = Parser(
        """
3 + (5 * (10 - 2)
"""
    )

    ast.Program.Compress()
    assert len(ast.Program) == 3
    assert len(ast.Program[2]) == 3
    assert len(ast.Program[2][2]) == 3


def test_compress_redundant_closing_parenthesis():
    ast = Parser(
        """ 
3 + 5) * (10 - 2)
"""
    )

    ast.Program.Compress()
    assert len(ast.Program) == 6
    assert len(ast.Program[5]) == 3


def test_compress_block_parenthesis():
    ast = Parser(
        """
{
    x = 5;
    y = x + 3;
}
"""
    )

    ast.Program.Compress()
    assert len(ast.Program) == 1
    assert len(ast.Program[0]) == 10


def test_compress_nested_block_parenthesis():
    ast = Parser(
        """
{
    x = 5;
    {
        y = x + 3;
    }
}
"""
    )

    ast.Program.Compress()
    assert len(ast.Program) == 1
    assert len(ast.Program[0]) == 5
    assert len(ast.Program[0][4]) == 6


def test_indexing_parenthesis():
    ast = Parser(
        """
array[5 + 3] = 10;
value = array[2 * 4];
"""
    )

    ast.Program.Compress()
    assert len(ast.Program) == 10


def test_compress_mixed_parenthesis():
    ast = Parser(
        """
    t = (3 + 5) * (10 - 2);
{
    x = (5 + 3) * 2;
    y = x - (4 / 2);
}
"""
    )

    ast.Program.Compress()
    assert len(ast.Program) == 7
    assert len(ast.Program[-1]) == 12


def test_compress_nested_parenthesis_2():
    ast = Parser(
        """
        print((test[2 + 3.10] + (4.3 - 1)) * 5);
"""
    )

    ast.Program.Compress()
    assert len(ast.Program) == 3
    assert len(ast.Program[1]) == 3
    assert len(ast.Program[1][0]) == 4


def test_label_node_type():
    ast = Parser(
        """
3 + 5
"""
    )

    ast.Program.Compress()
    ast.Program.Parse()

    ProgramAssertion(
        OperationAssertion(
            "+",
            AtomicAssertion(TokenType.INTEGER, 3),
            AtomicAssertion(TokenType.INTEGER, 5),
        )
    ).Assert(ast.Program)


def test_label_node_with_parenthesis():
    ast = Parser(
        """
(3 + 5) * (10 - 2)
"""
    )

    ast.Program.Compress()
    ast.Program.Parse()

    ProgramAssertion(
        OperationAssertion(
            "*",
            ExpressBlockionAsserion(
                OperationAssertion(
                    "+",
                    AtomicAssertion(TokenType.INTEGER, 3),
                    AtomicAssertion(TokenType.INTEGER, 5),
                ),
            ),
            ExpressBlockionAsserion(
                OperationAssertion(
                    "-",
                    AtomicAssertion(TokenType.INTEGER, 10),
                    AtomicAssertion(TokenType.INTEGER, 2),
                ),
            ),
        ),
    ).Assert(ast.Program)


def test_label_multiple_and_divide_before_add_and_subtract():
    ast = Parser(
        """
3 + 5 * 10
"""
    )

    ast.Program.Compress()
    ast.Program.Parse()

    ProgramAssertion(
        OperationAssertion(
            "+",
            AtomicAssertion(TokenType.INTEGER, 3),
            OperationAssertion(
                "*",
                AtomicAssertion(TokenType.INTEGER, 5),
                AtomicAssertion(TokenType.INTEGER, 10),
            ),
        ),
    ).Assert(ast.Program)


def test_label_multiple_and_divide():
    ast = Parser(
        """ 
3 * 5 / 10
"""
    )

    ast.Program.Compress()
    ast.Program.Parse()

    ProgramAssertion(
        OperationAssertion(
            "/",
            OperationAssertion(
                "*",
                AtomicAssertion(TokenType.INTEGER, 3),
                AtomicAssertion(TokenType.INTEGER, 5),
            ),
            AtomicAssertion(TokenType.INTEGER, 10),
        ),
    ).Assert(ast.Program)


def test_label_complex_expression():
    ast = Parser(
        """ 
(3 + 5) * (10 - 2) / (4 + 6) - 7
"""
    )

    ast.Program.Compress()
    ast.Program.Parse()

    ProgramAssertion(
        OperationAssertion(
            "-",
            OperationAssertion(
                "/",
                OperationAssertion(
                    "*",
                    ExpressBlockionAsserion(
                        OperationAssertion(
                            "+",
                            AtomicAssertion(TokenType.INTEGER, 3),
                            AtomicAssertion(TokenType.INTEGER, 5),
                        ),
                    ),
                    ExpressBlockionAsserion(
                        OperationAssertion(
                            "-",
                            AtomicAssertion(TokenType.INTEGER, 10),
                            AtomicAssertion(TokenType.INTEGER, 2),
                        ),
                    ),
                ),
                ExpressBlockionAsserion(
                    OperationAssertion(
                        "+",
                        AtomicAssertion(TokenType.INTEGER, 4),
                        AtomicAssertion(TokenType.INTEGER, 6),
                    ),
                ),
            ),
            AtomicAssertion(TokenType.INTEGER, 7),
        ),
    ).Assert(ast.Program)


def test_label_invalid_expression_with_lack_of_right_operand():
    ast = Parser(
        """ 
3 + 5 *
"""
    )

    ast.Program.Compress()
    ast.Program.Parse()

    ProgramAssertion(
        OperationAssertion(
            "+",
            AtomicAssertion(TokenType.INTEGER, 3),
            OperationAssertion(
                "*",
                AtomicAssertion(TokenType.INTEGER, 5),
                InvalidAtomicAssertion(),
                error="Right side of operator '*' is invalid",
            ),
        ),
    ).Assert(ast.Program)


def test_label_invalid_expression_with_lack_of_left_operand():
    ast = Parser(
        """
3 + * 10
"""
    )

    ast.Program.Compress()
    ast.Program.Parse()

    ProgramAssertion(
        OperationAssertion(
            "+",
            AtomicAssertion(TokenType.INTEGER, 3),
            OperationAssertion(
                "*",
                InvalidAtomicAssertion(),
                AtomicAssertion(TokenType.INTEGER, 10),
                error="Left side of operator '*' is invalid",
            ),
        ),
    ).Assert(ast.Program)


def test_label_invalid_expression_with_both_operands_missing():
    ast = Parser(
        """
3 + *
"""
    )

    ast.Program.Compress()
    ast.Program.Parse()

    ProgramAssertion(
        OperationAssertion(
            "+",
            AtomicAssertion(TokenType.INTEGER, 3),
            OperationAssertion(
                "*",
                InvalidAtomicAssertion(),
                InvalidAtomicAssertion(),
                error="Both sides of operator '*' are invalid",
            ),
        ),
    ).Assert(ast.Program)


def test_label_invalid_expression_with_multiple_operators_and_multiple_minus_operator():
    ast = Parser(
        """
3 + * - 5
        """
    )

    ast.Program.Compress()
    ast.Program.Parse()
    print(ast.Program)

    ProgramAssertion(
        OperationAssertion(
            "-",
            OperationAssertion(
                "+",
                AtomicAssertion(TokenType.INTEGER, 3),
                OperationAssertion(
                    "*",
                    InvalidAtomicAssertion(),
                    InvalidAtomicAssertion(),
                    error="Both sides of operator '*' are invalid",
                ),
            ),
            AtomicAssertion(TokenType.INTEGER, 5),
        ),
    ).Assert(ast.Program)
