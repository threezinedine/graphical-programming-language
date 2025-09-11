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

        Vector<Ref<Node>> CompressWithBrackets(const Vector<Ref<Node>> &nodes,
                                               NodeType blockType,
                                               const String &bracketOpen,
                                               const String &bracketClose);

        void ParseUnaryOperations(const Vector<Ref<Node>> &sourceNodes,
                                  Vector<Ref<Node>> &outNodes,
                                  b8 &hasAnyChange,
                                  const std::set<String> &operators);

        void ParseOperations(const Vector<Ref<Node>> &sourceNodes,
                             Vector<Ref<Node>> &outNodes,
                             b8 &hasAnyChange,
                             const std::set<String> &operators);

        void ParseStatements(const Vector<Ref<Node>> &sourceNodes,
                             Vector<Ref<Node>> &outNodes);

        void ParseIfStatements(const Vector<Ref<Node>> &sourceNodes,
                               Vector<Ref<Node>> &outNodes,
                               b8 &hasAnyChange);

        void ParseFunctionCall(const Vector<Ref<Node>> &sourceNodes,
                               Vector<Ref<Node>> &outNodes,
                               b8 &hasAnyChange);

        void ParseExpressions(const Vector<Ref<Node>> &sourceNodes,
                              Vector<Ref<Node>> &outNodes, b8 &containsComma);

    private:
        NodeType m_type = NodeType::INVALID;
        String m_content;
        Vector<Ref<Node>> m_children;
    };
} // namespace ntt
