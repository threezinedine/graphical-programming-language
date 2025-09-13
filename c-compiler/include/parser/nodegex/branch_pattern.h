#pragma once
#include "pch.h"
#include "node_pattern.h"

namespace ntt
{
    class BranchPattern : public NodePattern
    {
    public:
        BranchPattern(const Vector<Ref<NodePattern>> &patterns, NodegexQuantifier quantifier = NodegexQuantifier::ONCE);
        BranchPattern(const BranchPattern &other) = default;
        ~BranchPattern() override;

    protected:
        b8 MatchImpl(const Vector<Ref<Node>> &inNodes, u32 &startIndex) override;

    private:
        Vector<Ref<NodePattern>> m_patterns;
    };

    class BranchPatternBuilder : public NodePatternBuilder
    {
    public:
        BranchPatternBuilder(NodegexQuantifier quantifier = NodegexQuantifier::ONCE);
        BranchPatternBuilder(const BranchPatternBuilder &other) = default;
        ~BranchPatternBuilder();

        Ref<BranchPatternBuilder> AddPattern(Ref<NodePatternBuilder> pattern);
        Ref<NodePattern> Build();

    private:
        Vector<Ref<NodePatternBuilder>> m_patterns;
        NodegexQuantifier m_quantifier = NodegexQuantifier::ONCE;
    };
} // namespace ntt
