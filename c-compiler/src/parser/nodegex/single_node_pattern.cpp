#include "parser/nodegex/single_node_pattern.h"

namespace ntt
{
    SingleNodePattern::SingleNodePattern(const NodePatternPair &pair)
        : m_pair(pair)
    {
    }

    SingleNodePattern::~SingleNodePattern()
    {
    }

    u32 SingleNodePattern::GetPatternLength()
    {
        return 1;
    }

    b8 SingleNodePattern::Match(const Vector<Ref<Node>> &inNodes, u32 startIndex)
    {
        Ref<Node> currentNode = inNodes[startIndex];
        if (currentNode->GetType() != m_pair.type)
        {
            return NTT_FALSE;
        }

        return NTT_TRUE;
    }
} // namespace ntt
