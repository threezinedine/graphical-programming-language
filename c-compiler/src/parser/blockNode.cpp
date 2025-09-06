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
        m_children = CompressWithBrackets(m_children, NodeType::EXPRESSION, "(", ")");
    }

    Vector<Ref<Node>> BlockNode::CompressWithBrackets(const Vector<Ref<Node>> &nodes,
                                                      NodeType blockType,
                                                      const String &bracketOpen,
                                                      const String &bracketClose)
    {
        Vector<Ref<Node>> compressedNodes;
        u32 numberOfNodes = u32(nodes.size());
        u32 nodeIndex = 0;

        u8 bracketDepth = 0;

        Vector<Ref<Node>> temporaryCompressedNode;
        temporaryCompressedNode.clear();

        while (nodeIndex < numberOfNodes)
        {
            const Ref<Node> &currentNode = nodes[nodeIndex];

            if (currentNode->GetType() != NodeType::ATOMIC)
            {
                compressedNodes.push_back(currentNode);
                nodeIndex++;
                continue;
            }

            Atomic *atomicNode = dynamic_cast<Atomic *>(currentNode.get());
            const Token &currentNodeToken = atomicNode->GetToken();

            if (currentNodeToken.GetType() != TokenType::BRACKET ||
                (currentNodeToken.GetValue<std::string>() != bracketOpen &&
                 currentNodeToken.GetValue<std::string>() != bracketClose))
            {
                if (bracketDepth == 0)
                {
                    compressedNodes.push_back(currentNode);
                }
                else
                {
                    temporaryCompressedNode.push_back(currentNode);
                }
                nodeIndex++;
                continue;
            }

            if (currentNodeToken.GetValue<std::string>() == bracketOpen)
            {
                if (bracketDepth > 0)
                {
                    temporaryCompressedNode.push_back(currentNode);
                }
                bracketDepth++;
                nodeIndex++;
                continue;
            }
            else if (currentNodeToken.GetValue<std::string>() == bracketClose)
            {
                bracketDepth--;
                if (bracketDepth == 0)
                {
                    Ref<BlockNode> newBlockNode = CreateRef<BlockNode>(blockType, temporaryCompressedNode);
                    newBlockNode->Compress();
                    compressedNodes.push_back(newBlockNode);
                    nodeIndex += u32(temporaryCompressedNode.size() + 1);
                    temporaryCompressedNode.clear();
                }
                else
                {
                    temporaryCompressedNode.push_back(currentNode);
                    nodeIndex++;
                }
            }
        }

        return compressedNodes;
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