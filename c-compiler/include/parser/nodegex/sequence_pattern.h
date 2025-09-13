#pragma once
#include "pch.h"
#include "node_pattern.h"

namespace ntt
{
    class SequencePattern : public NodePattern
    {
    public:
        SequencePattern(const Vector<Ref<NodePattern>> &patterns, NodegexQuantifier quantifier = NodegexQuantifier::ONCE);
        SequencePattern(const SequencePattern &other) = default;
        ~SequencePattern() override;

    protected:
        b8 MatchImpl(const Vector<Ref<Node>> &inNodes, u32 &startIndex) override;

    private:
        Vector<Ref<NodePattern>> m_patterns;
    };

    class SequencePatternBuilder : public NodePatternBuilder
    {
    public:
        SequencePatternBuilder(NodegexQuantifier quantifier = NodegexQuantifier::ONCE);
        SequencePatternBuilder(const SequencePatternBuilder &other) = default;
        ~SequencePatternBuilder();

        Ref<SequencePatternBuilder> AddPattern(Ref<NodePatternBuilder> pattern);
        Ref<NodePattern> Build() override;

    private:
        Vector<Ref<NodePatternBuilder>> m_patterns;
    };
} // namespace ntt
