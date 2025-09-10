#pragma once
#include "pch.h"
#include "node.h"

namespace ntt
{
    class IfStatementNode : public Node
    {
    public:
        IfStatementNode(const Ref<Node> &conditionNode, const Ref<Node> &blockNode);
        ~IfStatementNode() override;

        inline NodeType GetType() const override { return NodeType::IF_STATEMENT; }

        JSON ToJSON() override;
        void Compress() override;
        void Parse() override;

    public:
        inline const Ref<Node> &GetConditionNode() const { return m_conditionNode; }
        inline const Ref<Node> &GetBlockNode() const { return m_blockNode; }

        inline Ref<Node> &GetConditionNode() { return m_conditionNode; }
        inline Ref<Node> &GetBlockNode() { return m_blockNode; }

    private:
        Ref<Node> m_conditionNode;
        Ref<Node> m_blockNode;
    };
} // namespace ntt
