#pragma once
#include "pch.h"
#include "node.h"

namespace ntt
{
    class UnaryOperationNode : public Node
    {
    public:
        UnaryOperationNode(Ref<Node> operatorNode, Ref<Node> operandNode);
        ~UnaryOperationNode();

        inline NodeType GetType() const override { return NodeType::UNARY_OPERATION; }

        JSON ToJSON() override;
        void Compress() override;
        void Parse() override;

    public:
        inline const Ref<Node> &GetOperand() const { return m_operand; }
        inline const Ref<Node> &GetOperator() const { return m_operator; }

    private:
        Ref<Node> m_operand;
        Ref<Node> m_operator;
    };
}
