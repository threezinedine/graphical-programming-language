#pragma once
#include "pch.h"
#include "node.h"

namespace ntt
{
    class OperationNode : public Node
    {
    public:
        OperationNode(const Ref<Node> &operatorNode,
                      const Ref<Node> &leftOperand,
                      const Ref<Node> &rightOperand);
        ~OperationNode() override;

        inline NodeType GetType() const override { return NodeType::OPERATION; }
        JSON ToJSON() override;

        void Compress() override;
        void Parse() override;

    public:
        inline const Ref<Node> &GetOperator() const { return m_operatorNode; }
        inline const Ref<Node> &GetLeftOperand() const { return m_leftOperand; }
        inline const Ref<Node> &GetRightOperand() const { return m_rightOperand; }

    private:
        Ref<Node> m_operatorNode;
        Ref<Node> m_leftOperand;
        Ref<Node> m_rightOperand;
    };
} // namespace ntt
