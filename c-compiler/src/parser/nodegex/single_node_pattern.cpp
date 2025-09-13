#include "parser/nodegex/single_node_pattern.h"
#include "parser/atomic.h"

namespace ntt
{
    SingleNodePattern::SingleNodePattern(const NodePatternPair &pair, NodegexQuantifier quantifier)
        : NodePattern(quantifier), m_pair(pair)
    {
    }

    SingleNodePattern::~SingleNodePattern()
    {
    }

    b8 SingleNodePattern::MatchImpl(const Vector<Ref<Node>> &inNodes, u32 &startIndex)
    {
        if (startIndex >= inNodes.size())
        {
            return NTT_FALSE;
        }

        Ref<Node> currentNode = inNodes[startIndex];
        if (currentNode->GetType() != m_pair.type)
        {
            return NTT_FALSE;
        }

        if (currentNode->GetType() != NodeType::ATOMIC)
        {
            startIndex++;
            return NTT_TRUE;
        }

        Ref<Atomic> atomicNode = std::dynamic_pointer_cast<Atomic>(currentNode);
        const Token &token = atomicNode->GetToken();

        b8 isMatched = m_pair.values.empty();

        for (const Token &value : m_pair.values)
        {
            if (token.GetType() != value.GetType())
            {
                continue;
            }

            switch (token.GetType())
            {
            case TokenType::INTEGER:
                if (token.GetValue<u32>() == value.GetValue<u32>())
                {
                    isMatched = NTT_TRUE;
                }
                break;
            case TokenType::BOOLEAN:
                if (token.GetValue<b8>() == value.GetValue<b8>())
                {
                    isMatched = NTT_TRUE;
                }
                break;
            case TokenType::FLOAT:
                if (token.GetValue<f32>() == value.GetValue<f32>())
                {
                    isMatched = NTT_TRUE;
                }
                break;
            default:
                if (token.GetValue<String>() == value.GetValue<String>())
                {
                    isMatched = NTT_TRUE;
                }
            }
        }

        if (isMatched)
        {
            startIndex++;
        }

        return isMatched;
    }

    SingleNodePatternBuilder::SingleNodePatternBuilder(NodeType type, NodegexQuantifier quantifier)
        : NodePatternBuilder(quantifier)
    {
        m_pair.type = type;
    }

    SingleNodePatternBuilder::~SingleNodePatternBuilder()
    {
    }
} // namespace ntt
