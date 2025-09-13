#include "parser/nodegex/sequence_pattern.h"

namespace ntt
{
    SequencePattern::SequencePattern(const Vector<Ref<NodePattern>> &patterns, NodegexQuantifier quantifier)
        : NodePattern(quantifier), m_patterns(patterns)
    {
    }

    SequencePattern::~SequencePattern()
    {
    }

    b8 SequencePattern::MatchImpl(const Vector<Ref<Node>> &inNodes, u32 &startIndex)
    {
        u32 originalIndex = startIndex;

        for (const Ref<NodePattern> &pattern : m_patterns)
        {
            if (!pattern->Match(inNodes, startIndex))
            {
                startIndex = originalIndex;
                return NTT_FALSE;
            }
        }

        return NTT_TRUE;
    }

    SequencePatternBuilder::SequencePatternBuilder(NodegexQuantifier quantifier)
        : NodePatternBuilder(quantifier)
    {
    }

    SequencePatternBuilder::~SequencePatternBuilder()
    {
    }

    Ref<SequencePatternBuilder> SequencePatternBuilder::AddPattern(Ref<NodePatternBuilder> pattern)
    {
        m_patterns.push_back(pattern);
        return CreateRef<SequencePatternBuilder>(*this);
    }

    Ref<NodePattern> SequencePatternBuilder::Build()
    {
        Vector<Ref<NodePattern>> builtPatterns;
        for (const Ref<NodePatternBuilder> &patternBuilder : m_patterns)
        {
            builtPatterns.push_back(patternBuilder->Build());
        }

        return CreateRef<SequencePattern>(builtPatterns, GetQuantifier());
    }
}