#pragma once
#include "pch.h"
#include "node.h"

namespace ntt
{
    class FunctionCallNode : public Node
    {
    public:
        FunctionCallNode(const Ref<Node> &function, const Vector<Ref<Node>> &arguments);
        ~FunctionCallNode() override;

        inline NodeType GetType() const override { return NodeType::FUNCTION_CALL; }

        JSON ToJSON() override;
        void Compress() override;
        void Parse() override;

    public:
        const Ref<Node> &GetFunction() const { return m_function; }
        const Vector<Ref<Node>> &GetArguments() const { return m_arguments; }

    private:
        Ref<Node> m_function;
        Vector<Ref<Node>> m_arguments;
    };
} // namespace ntt
