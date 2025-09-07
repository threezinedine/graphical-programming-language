#pragma once
#include "pch.h"
#include "node.h"

namespace ntt
{
    class InvalidNode : public Node
    {
    public:
        InvalidNode(const String &content = "");
        ~InvalidNode() override;

        inline NodeType GetType() const override { return NodeType::INVALID; }

        JSON ToJSON() override;

        void Compress() override;
        void Parse() override;

    private:
        String m_content;
    };
} // namespace ntt
