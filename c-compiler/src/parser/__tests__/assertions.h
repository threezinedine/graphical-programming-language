#pragma once
#include "compiler.h"

using namespace ntt;

class DelayAssertion
{
public:
    virtual void Assert(Ref<Node> node) = 0;
};

class BlockAssertion : public DelayAssertion
{
public:
    BlockAssertion(NodeType blockType, const Vector<Ref<DelayAssertion>> &assertions)
        : m_blockType(blockType), m_assertions(assertions)
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
    AtomicAssertion(TokenType expectType, u32 expectValue)
        : m_expectType(expectType)
    {
        m_expectValue.numberValue.intValue = expectValue;
    }
    AtomicAssertion(TokenType expectType, f32 expectValue)
        : m_expectType(expectType)
    {
        m_expectValue.numberValue.floatValue = expectValue;
    }
    AtomicAssertion(TokenType expectType, const String &expectValue)
        : m_expectType(expectType)
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

#define EXPRESSION_ASSERTION(...) \
    CreateRef<BlockAssertion>(NodeType::EXPRESSION, Vector<Ref<DelayAssertion>>{__VA_ARGS__})

#define ATOMIC_ASSERTION(type, value) \
    CreateRef<AtomicAssertion>(type, value)