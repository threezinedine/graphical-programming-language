#include "parser/atomic.h"

namespace ntt
{
    Atomic::Atomic(NodeType type, const Token &token)
        : m_token(token)
    {
    }

    Atomic::~Atomic()
    {
    }

    JSON Atomic::ToJSON()
    {
        JSON json;
        json["type"] = "Atomic";
        json["token"] = m_token.ToJSON();
        return json;
    }

    void Atomic::Compress()
    {
        // Nothing to compress in an atomic node
    }

    void Atomic::Parse()
    {
        // Parsing logic for atomic nodes can be implemented here
    }
}