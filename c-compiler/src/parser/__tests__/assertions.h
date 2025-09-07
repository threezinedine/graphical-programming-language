#pragma once
#include "compiler.h"

using namespace ntt;

class DelayAssertion
{
public:
    DelayAssertion() : m_error(ErrorType::NO_ERROR) {}
    DelayAssertion(ErrorType error) : m_error(error) {}
    virtual ~DelayAssertion() = default;

    virtual void Assert(Ref<Node> node) = 0;

    virtual b8 HasError() const { return m_error != ErrorType::NO_ERROR; }
    virtual ErrorType GetError() const { return m_error; }

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

    void Assert(Ref<Node> node) override;

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

    ~AtomicAssertion() = default;

    void Assert(Ref<Node> node) override;

private:
    TokenType m_expectType;
    TokenValue m_expectValue;
};

#define COMPRESS_ONLY_DEFINE(content)                \
    BlockNode blockNode(NodeType::PROGRAM, content); \
    blockNode.Compress();

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