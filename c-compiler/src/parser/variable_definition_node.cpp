#include "parser/variable_definition_node.h"

namespace ntt
{
    VariableDefinitionNode::VariableDefinitionNode(Ref<Node> defineType, Ref<Node> name, Ref<Node> type, Ref<Node> defaultValue)
        : m_defineType(defineType), m_name(name), m_type(type), m_defaultValue(defaultValue)
    {
    }

    VariableDefinitionNode::~VariableDefinitionNode()
    {
    }

    JSON VariableDefinitionNode::ToJSON()
    {
        JSON json;
        json["type"] = "VariableDefinitionNode";
        json["defineType"] = m_defineType->ToJSON();
        json["name"] = m_name->ToJSON();
        json["type"] = m_type->ToJSON();
        if (m_defaultValue)
        {
            json["defaultValue"] = m_defaultValue->ToJSON();
        }
        return json;
    }

    void VariableDefinitionNode::Compress()
    {
        if (m_type)
        {
            m_type->Compress();
        }

        if (m_name)
        {
            m_name->Compress();
        }

        if (m_defineType)
        {
            m_defineType->Compress();
        }

        if (m_defaultValue)
        {
            m_defaultValue->Compress();
        }
    }

    void VariableDefinitionNode::Parse()
    {
        if (m_type)
        {
            m_type->Parse();
        }

        if (m_name)
        {
            m_name->Parse();
        }

        if (m_defineType)
        {
            m_defineType->Parse();
        }

        if (m_defaultValue)
        {
            m_defaultValue->Parse();
        }
    }
} // namespace ntt
