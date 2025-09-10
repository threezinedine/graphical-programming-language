#include "parser/if_statement.h"

namespace ntt
{
    IfStatementNode::IfStatementNode(const Ref<Node> &conditionNode, const Ref<Node> &blockNode, const Ref<Node> &elseBlockNode)
        : m_conditionNode(conditionNode), m_blockNode(blockNode), m_elseBlockNode(elseBlockNode)
    {
    }

    IfStatementNode::~IfStatementNode()
    {
    }

    JSON IfStatementNode::ToJSON()
    {
        JSON json;
        json["type"] = "IfStatementNode";
        json["condition"] = m_conditionNode->ToJSON();
        json["block"] = m_blockNode->ToJSON();

        return json;
    }

    void IfStatementNode::Compress()
    {
        if (m_conditionNode)
        {
            m_conditionNode->Compress();
        }

        if (m_blockNode)
        {
            m_blockNode->Compress();
        }
    }

    void IfStatementNode::Parse()
    {
        if (m_conditionNode)
        {
            m_conditionNode->Parse();
        }

        if (m_blockNode)
        {
            m_blockNode->Parse();
        }
    }
} // namespace ntt
