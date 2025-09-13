#include "parser/nodegex/branch_pattern.h"

namespace ntt
{
    BranchPattern::BranchPattern(const Vector<Ref<NodePattern>> &patterns, NodegexQuantifier quantifier)
        : NodePattern(quantifier), m_patterns(patterns)
    {
    }

    BranchPattern::~BranchPattern()
    {
    }

    b8 BranchPattern::MatchImpl(const Vector<Ref<Node>> &inNodes, u32 &startIndex)
    {
        u32 originalIndex = startIndex;

        for (const Ref<NodePattern> &pattern : m_patterns)
        {
            if (pattern->Match(inNodes, startIndex))
            {
                return NTT_TRUE;
            }
            startIndex = originalIndex;
        }

        return NTT_FALSE;
    }

    BranchPatternBuilder::BranchPatternBuilder(NodegexQuantifier quantifier)
        : NodePatternBuilder(quantifier)
    {
    }

    BranchPatternBuilder::~BranchPatternBuilder()
    {
    }

    Ref<BranchPatternBuilder> BranchPatternBuilder::AddPattern(Ref<NodePatternBuilder> pattern)
    {
        m_patterns.push_back(pattern);
        return CreateRef<BranchPatternBuilder>(*this);
    }

    Ref<NodePattern> BranchPatternBuilder::Build()
    {
        Vector<Ref<NodePattern>> builtPatterns;
        for (const Ref<NodePatternBuilder> &patternBuilder : m_patterns)
        {
            builtPatterns.push_back(patternBuilder->Build());
        }

        return CreateRef<BranchPattern>(builtPatterns, GetQuantifier());
    }
} // namespace ntt
