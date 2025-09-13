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
        Vector<Token> values;
    };

    /**
     * The pattern which checking if the next single node is matched against the pattern.
     */
    class SingleNodePattern : public NodePattern
    {
    public:
        SingleNodePattern(const NodePatternPair &pair);
        SingleNodePattern(const SingleNodePattern &other) = default;
        ~SingleNodePattern() override;

        b8 Match(const Vector<Ref<Node>> &inNodes, u32 &startIndex) override;

    private:
        NodePatternPair m_pair;
    };

    /**
     * Used for creating the SingleNodePattern objects.
     */
    class SingleNodePatternBuilder : public NodePatternBuilder
    {
    public:
        SingleNodePatternBuilder(NodeType type);
        SingleNodePatternBuilder(const SingleNodePatternBuilder &other) = default;
        ~SingleNodePatternBuilder();

        template <typename T>
        SingleNodePatternBuilder AddValue(TokenType tokenType, T value)
        {
            NTT_ASSERT(m_pair.type == NodeType::ATOMIC);

            Token token(tokenType, 0);
            token.SetValue(value);
            m_pair.values.push_back(token);
            return *this;
        }

        inline Ref<NodePattern> Build() { return CreateRef<SingleNodePattern>(m_pair); }

    private:
        NodePatternPair m_pair;
    };

} // namespace ntt