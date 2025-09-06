#pragma once
#include "compiler.h"

using namespace ntt;

class DelayAssertion
{
public:
    virtual void Assert(Ref<Node> node) = 0;
};

class ProgramAssertion : public DelayAssertion
{
public:
    ProgramAssertion(const Vector<Ref<DelayAssertion>> &assertions)
        : m_assertions(assertions)
    {
    }

    ~ProgramAssertion() = default;

    void Assert(Ref<Node> node) override;

private:
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