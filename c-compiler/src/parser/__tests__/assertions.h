#pragma once
#include "compiler.h"

using namespace ntt;

class DelayAssertion
{
public:
    DelayAssertion() : m_error(ErrorType::NO_ERROR) {}
    DelayAssertion(ErrorType error) : m_error(error) {}
    virtual ~DelayAssertion() = default;

    void Assert(Ref<Node> node);

    virtual b8 HasError() const { return m_error != ErrorType::NO_ERROR; }
    virtual ErrorType GetError() const { return m_error; }

protected:
    virtual void _Assert(Ref<Node> node) = 0;

private:
    ErrorType m_error;
};

class BlockAssertion : public DelayAssertion
{
public:
    BlockAssertion(NodeType blockType, const Vector<Ref<DelayAssertion>> &assertions, ErrorType error = ErrorType::NO_ERROR)
        : DelayAssertion(error), m_blockType(blockType), m_assertions(assertions)
    {
    }

    ~BlockAssertion() = default;

protected:
    void _Assert(Ref<Node> node) override;

private:
    NodeType m_blockType;
    Vector<Ref<DelayAssertion>> m_assertions;
};

class AtomicAssertion : public DelayAssertion
{
public:
    AtomicAssertion(TokenType expectType, u32 expectValue, ErrorType error = ErrorType::NO_ERROR)
        : DelayAssertion(error), m_expectType(expectType)
    {
        m_expectValue.numberValue.intValue = expectValue;
    }

    AtomicAssertion(TokenType expectType, f32 expectValue, ErrorType error = ErrorType::NO_ERROR)
        : DelayAssertion(error), m_expectType(expectType)
    {
        m_expectValue.numberValue.floatValue = expectValue;
    }

    AtomicAssertion(TokenType expectType, const String &expectValue, ErrorType error = ErrorType::NO_ERROR)
        : DelayAssertion(error), m_expectType(expectType)
    {
        m_expectValue.stringValue = expectValue;
    }

    AtomicAssertion(TokenType expectType, const char *expectValue, ErrorType error = ErrorType::NO_ERROR)
        : DelayAssertion(error), m_expectType(expectType)
    {
        m_expectValue.stringValue = expectValue;
    }

    AtomicAssertion(TokenType expectType, b8 expectValue, ErrorType error = ErrorType::NO_ERROR)
        : DelayAssertion(error), m_expectType(expectType)
    {
        m_expectValue.numberValue.boolValue = expectValue;
    }

    ~AtomicAssertion() = default;

protected:
    void _Assert(Ref<Node> node) override;

private:
    TokenType m_expectType;
    TokenValue m_expectValue;
};

class InvalidAssertion : public DelayAssertion
{
public:
    InvalidAssertion(ErrorType error = ErrorType::NO_ERROR)
        : DelayAssertion(error) {}
    ~InvalidAssertion() = default;

protected:
    void _Assert(Ref<Node> node) override;
};

class UnaryOperationAssertion : public DelayAssertion
{
public:
    UnaryOperationAssertion(Ref<DelayAssertion> operation, Ref<DelayAssertion> operand,
                            ErrorType error = ErrorType::NO_ERROR)
        : DelayAssertion(error), m_operation(operation), m_operand(operand) {}

    ~UnaryOperationAssertion() = default;

protected:
    void _Assert(Ref<Node> node) override;

private:
    Ref<DelayAssertion> m_operation;
    Ref<DelayAssertion> m_operand;
};

class OperationAssertion : public DelayAssertion
{
public:
    OperationAssertion(Ref<DelayAssertion> operatorNode,
                       Ref<DelayAssertion> leftOperand,
                       Ref<DelayAssertion> rightOperand,
                       ErrorType error = ErrorType::NO_ERROR)
        : DelayAssertion(error), m_operatorNode(operatorNode),
          m_leftOperand(leftOperand), m_rightOperand(rightOperand) {}

    ~OperationAssertion() = default;

protected:
    void _Assert(Ref<Node> node) override;

private:
    Ref<DelayAssertion> m_operatorNode;
    Ref<DelayAssertion> m_leftOperand;
    Ref<DelayAssertion> m_rightOperand;
};

class IfStatementAssertion : public DelayAssertion
{
public:
    IfStatementAssertion(Ref<DelayAssertion> condition,
                         Ref<DelayAssertion> block,
                         Ref<DelayAssertion> elseBlock,
                         ErrorType error = ErrorType::NO_ERROR)
        : DelayAssertion(error), m_condition(condition), m_block(block), m_elseBlock(elseBlock) {}

    ~IfStatementAssertion() = default;

protected:
    void _Assert(Ref<Node> node) override;

private:
    Ref<DelayAssertion> m_condition;
    Ref<DelayAssertion> m_block;
    Ref<DelayAssertion> m_elseBlock;
};

#define COMPRESS_ONLY_DEFINE(content)                \
    BlockNode blockNode(NodeType::PROGRAM, content); \
    blockNode.Compress();

#define PARSE_DEFINE(content)     \
    COMPRESS_ONLY_DEFINE(content) \
    blockNode.Parse();

#define PROGRAM_ASSERTION(...)                                                             \
    CreateRef<BlockAssertion>(NodeType::PROGRAM, Vector<Ref<DelayAssertion>>{__VA_ARGS__}) \
        ->Assert(CreateRef<BlockNode>(blockNode));

#define PROGRAM_ASSERTION_ERR(err, ...)                                                           \
    CreateRef<BlockAssertion>(NodeType::PROGRAM, Vector<Ref<DelayAssertion>>{__VA_ARGS__} i, err) \
        ->Assert(CreateRef<BlockNode>(blockNode));

#define EXPRESSION_ASSERTION(...) \
    CreateRef<BlockAssertion>(NodeType::EXPRESSION, Vector<Ref<DelayAssertion>>{__VA_ARGS__})

#define EXPRESSION_ASSERTION_ERR(err, ...) \
    CreateRef<BlockAssertion>(NodeType::EXPRESSION, Vector<Ref<DelayAssertion>>{__VA_ARGS__}, err)

#define STATEMENT_ASSERTION(...) \
    CreateRef<BlockAssertion>(NodeType::STATEMENT, Vector<Ref<DelayAssertion>>{__VA_ARGS__})

#define STATEMENT_ASSERTION_ERR(err, ...) \
    CreateRef<BlockAssertion>(NodeType::STATEMENT, Vector<Ref<DelayAssertion>>{__VA_ARGS__}, err)

#define IF_STATEMENT_ASSERTION(condition, block, elseBlock) \
    CreateRef<IfStatementAssertion>(condition, block, elseBlock)

#define IF_STATEMENT_ASSERTION_ERR(err, condition, block, elseBlock) \
    CreateRef<IfStatementAssertion>(condition, block, elseBlock, err)

#define INDEX_ASSERTION(...) \
    CreateRef<BlockAssertion>(NodeType::INDEX, Vector<Ref<DelayAssertion>>{__VA_ARGS__})

#define INDEX_ASSERTION_ERR(err, ...) \
    CreateRef<BlockAssertion>(NodeType::INDEX, Vector<Ref<DelayAssertion>>{__VA_ARGS__}, err)

#define BLOCK_ASSERTION(...) \
    CreateRef<BlockAssertion>(NodeType::BLOCK, Vector<Ref<DelayAssertion>>{__VA_ARGS__})

#define BLOCK_ASSERTION_ERR(err, ...) \
    CreateRef<BlockAssertion>(NodeType::BLOCK, Vector<Ref<DelayAssertion>>{__VA_ARGS__}, err)

#define ATOMIC_ASSERTION(type, value) \
    CreateRef<AtomicAssertion>(type, value)

#define INVALID_ASSERTION() \
    CreateRef<InvalidAssertion>()

#define UNARY_ASSERTION(operation, operand) \
    CreateRef<UnaryOperationAssertion>(ATOMIC_ASSERTION(TokenType::OPERATOR, operation), operand)

#define UNARY_ASSERTION_ERR(err, operation, operand) \
    CreateRef<UnaryOperationAssertion>(ATOMIC_ASSERTION(TokenType::OPERATOR, operation), operand, err)

#define OPERATION_ASSERTION(leftOperand, operator, rightOperand) \
    CreateRef<OperationAssertion>(ATOMIC_ASSERTION(TokenType::OPERATOR, operator), leftOperand, rightOperand)

#define OPERATION_ASSERTION_ERR(err, leftOperand, operator, rightOperand) \
    CreateRef<OperationAssertion>(ATOMIC_ASSERTION(TokenType::OPERATOR, operator), leftOperand, rightOperand, err)