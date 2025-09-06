#pragma once
#include "pch.h"
#include "node.h"
#include "tokenizer/token.h"

namespace ntt
{
    class Atomic : public Node
    {
    public:
        Atomic(NodeType type, const Token &token);
        ~Atomic();

        inline NodeType GetType() const override { return NodeType::ATOMIC; }
        JSON ToJSON() override;

        void Compress() override;
        void Parse() override;

        /**
         * @return The token that this atomic node represents.
         */
        inline const Token &GetToken() const { return m_token; }

    private:
        Token m_token;
    };
} // namespace ntt
