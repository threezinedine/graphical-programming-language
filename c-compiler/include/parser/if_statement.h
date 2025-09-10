#pragma once
#include "pch.h"
#include "node.h"

namespace ntt
{
    class IfStatementNode : public Node
    {
    public:
        IfStatementNode(const Ref<Node> &conditionNode, const Ref<Node> &blockNode, const Ref<Node> &elseBlockNode);
        ~IfStatementNode() override;

        inline NodeType GetType() const override { return NodeType::IF_STATEMENT; }

        JSON ToJSON() override;
        void Compress() override;
        void Parse() override;

    public:
        inline const Ref<Node> &GetCondition() const { return m_conditionNode; }
        inline const Ref<Node> &GetBlock() const { return m_blockNode; }
        inline const Ref<Node> &GetElseBlock() const { return m_elseBlockNode; }

        inline Ref<Node> &GetCondition() { return m_conditionNode; }
        inline Ref<Node> &GetBlock() { return m_blockNode; }
        inline Ref<Node> &GetElseBlock() { return m_elseBlockNode; }

    private:
        Ref<Node> m_conditionNode;
        Ref<Node> m_blockNode;
        Ref<Node> m_elseBlockNode;
    };
} // namespace ntt
