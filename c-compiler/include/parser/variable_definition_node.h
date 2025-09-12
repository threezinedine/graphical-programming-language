#pragma once
#include "pch.h"
#include "node.h"

namespace ntt
{
    class VariableDefinitionNode : public Node
    {
    public:
        VariableDefinitionNode(Ref<Node> defineType, Ref<Node> name, Ref<Node> type, Ref<Node> defaultValue = nullptr);
        ~VariableDefinitionNode() override;

        NodeType GetType() const override { return NodeType::VARIABLE_DEFINITION_STATEMENT; }
        JSON ToJSON() override;
        void Compress() override;
        void Parse() override;

    public:
        inline Ref<Node> GetDefineType() const { return m_defineType; }
        inline Ref<Node> GetName() const { return m_name; }
        inline Ref<Node> GetTypeNode() const { return m_type; }
        inline Ref<Node> GetDefaultValue() const { return m_defaultValue; }

    private:
        Ref<Node> m_defineType;
        Ref<Node> m_name;
        Ref<Node> m_type;
        Ref<Node> m_defaultValue;
    };
} // namespace ntt
