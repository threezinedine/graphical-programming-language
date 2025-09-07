#include "parser/invalid.h"

namespace ntt
{
    InvalidNode::InvalidNode(const String &content)
        : m_content(content)
    {
    }

    InvalidNode::~InvalidNode()
    {
    }

    JSON InvalidNode::ToJSON()
    {
        JSON json;
        json["type"] = "InvalidNode";
        json["content"] = m_content;
        return json;
    }

    void InvalidNode::Compress()
    {
    }

    void InvalidNode::Parse()
    {
    }
} // namespace ntt
