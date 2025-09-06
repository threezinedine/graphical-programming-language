#include "parser/blockNode.h"

namespace ntt
{
    BlockNode::BlockNode(NodeType type, const String &content)
        : m_type(type), m_content(content)
    {
    }

    BlockNode::BlockNode(NodeType type, const Vector<Ref<Node>> &children)
        : m_type(type), m_children(children)
    {
    }

    BlockNode::~BlockNode()
    {
    }

    JSON BlockNode::ToJSON()
    {
        JSON json;
        json["type"] = "BlockNode";
        json["children"] = JSON::array();
        for (const auto &child : m_children)
        {
            json["children"].push_back(child->ToJSON());
        }

        return json;
    }

    void BlockNode::Compress()
    {
    }

    void BlockNode::Parse()
    {
    }
}