#pragma once
#include "pch.h"
#include "node.h"

namespace ntt
{
    /**
     * This type of node will contain multiple nodes inside it.
     */
    class BlockNode : public Node
    {
    public:
        BlockNode(NodeType type, const String &content);
        BlockNode(NodeType type, const Vector<Ref<Node>> &children);
        ~BlockNode();

        inline NodeType GetType() const override { return m_type; }
        JSON ToJSON() override;

        void Compress() override;
        void Parse() override;

    public:
        inline const Vector<Ref<Node>> &GetChildren() const { return m_children; }

    private:
        void TokenizeContent();

    private:
        NodeType m_type = NodeType::INVALID;
        String m_content;
        Vector<Ref<Node>> m_children;
    };
} // namespace ntt
