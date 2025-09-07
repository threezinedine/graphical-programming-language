#include "parser/operationNode.h"

namespace ntt
{
    OperationNode::OperationNode(const Ref<Node> &operatorNode,
                                 const Ref<Node> &leftOperand,
                                 const Ref<Node> &rightOperand)
        : m_operatorNode(operatorNode), m_leftOperand(leftOperand), m_rightOperand(rightOperand)
    {
    }

    OperationNode::~OperationNode()
    {
    }

    JSON OperationNode::ToJSON()
    {
        JSON json;
        json["type"] = "OperationNode";
        json["operator"] = m_operatorNode->ToJSON();
        json["leftOperand"] = m_leftOperand->ToJSON();
        json["rightOperand"] = m_rightOperand->ToJSON();
        return json;
    }

    void OperationNode::Compress()
    {
        if (m_operatorNode)
        {
            m_operatorNode->Compress();
        }

        if (m_leftOperand)
        {
            m_leftOperand->Compress();
        }

        if (m_rightOperand)
        {
            m_rightOperand->Compress();
        }
    }

    void OperationNode::Parse()
    {
        if (m_operatorNode)
        {
            m_operatorNode->Parse();
        }

        if (m_leftOperand)
        {
            m_leftOperand->Parse();
        }

        if (m_rightOperand)
        {
            m_rightOperand->Parse();
        }
    }
} // namespace ntt
