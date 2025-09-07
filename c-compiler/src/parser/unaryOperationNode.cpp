#include "parser/unaryOperationNode.h"

namespace ntt
{
    UnaryOperationNode::UnaryOperationNode(Ref<Node> operatorNode, Ref<Node> operandNode)
        : m_operator(operatorNode), m_operand(operandNode)
    {
    }

    UnaryOperationNode::~UnaryOperationNode()
    {
    }

    JSON UnaryOperationNode::ToJSON()
    {
        JSON json;
        json["type"] = "UnaryOperationNode";
        json["operator"] = m_operator->ToJSON();
        json["operand"] = m_operand->ToJSON();
        return json;
    }

    void UnaryOperationNode::Compress()
    {
        m_operator->Compress();
    }

    void UnaryOperationNode::Parse()
    {
        m_operator->Parse();
    }
} // namespace ntt
