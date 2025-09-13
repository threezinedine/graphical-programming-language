#include "parser/nodegex/node_pattern.h"

namespace ntt
{
    NodePattern::NodePattern(NodegexQuantifier quantifier)
        : m_quantifier(quantifier)
    {
    }

    NodePattern::~NodePattern()
    {
    }

    b8 NodePattern::Match(const Vector<Ref<Node>> &inNodes, u32 &startIndex)
    {
        switch (m_quantifier)
        {
        case NodegexQuantifier::ONCE:
            return MatchImpl(inNodes, startIndex);
        case NodegexQuantifier::ZERO_OR_ONCE:
        {
            MatchImpl(inNodes, startIndex);
            return NTT_TRUE;
        }
        case NodegexQuantifier::ZERO_OR_MORE:
        {
            while (MatchImpl(inNodes, startIndex))
                ;
            return NTT_TRUE;
        }
        case NodegexQuantifier::ONE_OR_MORE:
        {
            if (!MatchImpl(inNodes, startIndex))
            {
                return NTT_FALSE;
            }
            while (MatchImpl(inNodes, startIndex))
                ;
            return NTT_TRUE;
        }

        default:
            return NTT_FALSE;
            break;
        }
    }

    NodePatternBuilder::NodePatternBuilder(NodegexQuantifier quantifier)
        : m_quantifier(quantifier)
    {
    }

    NodePatternBuilder::~NodePatternBuilder()
    {
    }
} // namespace ntt
