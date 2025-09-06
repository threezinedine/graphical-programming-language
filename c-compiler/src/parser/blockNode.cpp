#include "parser/blockNode.h"
#include "tokenizer/tokenizer.h"
#include "parser/atomic.h"

namespace ntt
{
    BlockNode::BlockNode(NodeType type, const String &content)
        : m_type(type), m_content(content)
    {
        TokenizeContent();
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

    void BlockNode::TokenizeContent()
    {
        Tokenizer tokenizer(m_content);
        Vector<Token> tokens = tokenizer.GetTokens();

        for (const auto &token : tokens)
        {
            m_children.push_back(CreateRef<Atomic>(NodeType::ATOMIC, token));
        }
    }
}