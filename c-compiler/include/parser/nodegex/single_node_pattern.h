#pragma once
#include "pch.h"
#include "tokenizer/token.h"
#include "parser/node.h"
#include "node_pattern.h"

namespace ntt
{
    /**
     * The combination of node type and the matched content.
     */
    struct NodePatternPair
    {
        /**
         * This value determines the content category of the node.
         */
        NodeType type;

        /**
         * The list of the node's contents which are able to be matched against the pattern. If
         *      the node type is not a token, this value is not used. If the attribute is an empty
         *      list so that the pattern can match any content of the node.
         */
        Vector<TokenValue> values;
    };

    /**
     * The pattern which checking if the next single node is matched against the pattern.
     */
    class SingleNodePattern : public NodePattern
    {
    public:
        SingleNodePattern(const NodePatternPair &pair);
        ~SingleNodePattern() override;

        u32 GetPatternLength() override;
        b8 Match(const Vector<Ref<Node>> &inNodes, u32 startIndex) override;

    private:
        NodePatternPair m_pair;
    };
} // namespace ntt
