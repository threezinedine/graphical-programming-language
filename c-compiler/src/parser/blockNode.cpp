#include "parser/blockNode.h"
#include "tokenizer/tokenizer.h"
#include "parser/atomic.h"
#include <set>
#include "parser/unaryOperationNode.h"
#include "parser/invalid.h"
#include "parser/operationNode.h"

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
        m_children = CompressWithBrackets(m_children, NodeType::BLOCK, "{", "}");
        m_children = CompressWithBrackets(m_children, NodeType::EXPRESSION, "(", ")");
        m_children = CompressWithBrackets(m_children, NodeType::INDEX, "[", "]");
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
                if (bracketDepth == 0)
                {
                    // unmatched closing bracket, just push to the result
                    compressedNodes.push_back(currentNode);
                    nodeIndex++;
                    continue;
                }
                else if (bracketDepth == 1)
                {
                    Ref<BlockNode> newBlockNode = CreateRef<BlockNode>(blockType, temporaryCompressedNode);
                    newBlockNode->Compress();
                    compressedNodes.push_back(newBlockNode);
                    nodeIndex++;
                    temporaryCompressedNode.clear();
                }
                else
                {
                    temporaryCompressedNode.push_back(currentNode);
                    nodeIndex++;
                }
                bracketDepth--;
            }
        }

        if (temporaryCompressedNode.size() > 0)
        {
            Ref<BlockNode> newBlockNode = CreateRef<BlockNode>(blockType, temporaryCompressedNode);
            newBlockNode->Compress();
            newBlockNode->AddError(ErrorType::MISSING_END_BRACKET);
            compressedNodes.push_back(newBlockNode);
        }

        return compressedNodes;
    }

    static void ParseUnaryOperations(const Vector<Ref<Node>> &sourceNodes,
                                     Vector<Ref<Node>> &outNodes,
                                     b8 &foundUnary,
                                     const std::set<String> &operators);

    static void ParseOperations(const Vector<Ref<Node>> &sourceNodes,
                                Vector<Ref<Node>> &outNodes,
                                b8 &foundUnary,
                                const std::set<String> &operators);

    void BlockNode::Parse()
    {
        Vector<Ref<Node>> parsedNodes = m_children;
        b8 hasAnyModified = NTT_TRUE;

        while (hasAnyModified)
        {
            hasAnyModified = NTT_FALSE;
            Vector<Ref<Node>> newParsedNodes;
            ParseUnaryOperations(
                parsedNodes,
                newParsedNodes,
                hasAnyModified,
                std::set<String>{"!"});
            parsedNodes = newParsedNodes;
        }

        hasAnyModified = NTT_TRUE;

        while (hasAnyModified)
        {
            hasAnyModified = NTT_FALSE;
            Vector<Ref<Node>> newParsedNodes;
            ParseOperations(
                parsedNodes,
                newParsedNodes,
                hasAnyModified,
                std::set<String>{"+"});
            parsedNodes = newParsedNodes;
        }

        m_children = parsedNodes;
    }

    void ParseUnaryOperations(const Vector<Ref<Node>> &sourceNodes,
                              Vector<Ref<Node>> &outNodes,
                              b8 &foundUnary,
                              const std::set<String> &operators)
    {
        NTT_ASSERT(outNodes.empty());
        u32 numberOfSourceNodes = u32(sourceNodes.size());
        u32 sourceNodeIndex = 0;

        while (sourceNodeIndex < numberOfSourceNodes)
        {
            const Ref<Node> &currentNode = sourceNodes[sourceNodeIndex];

            if (currentNode->GetType() != NodeType::ATOMIC)
            {
                outNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            Atomic *atomicNode = dynamic_cast<Atomic *>(currentNode.get());
            const Token &currentNodeToken = atomicNode->GetToken();

            if (currentNodeToken.GetType() != TokenType::OPERATOR ||
                operators.find(currentNodeToken.GetValue<String>()) == operators.end())
            {
                outNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            if (sourceNodeIndex + 1 >= numberOfSourceNodes)
            {
                Ref<Node> newUnaryNode = CreateRef<UnaryOperationNode>(
                    currentNode, CreateRef<InvalidNode>());
                newUnaryNode->AddError(ErrorType::MISSING_RIGHT_OPERAND);
                outNodes.push_back(newUnaryNode);
                sourceNodeIndex++;
                continue;
            }

            const Ref<Node> &operandNode = sourceNodes[sourceNodeIndex + 1];

            if (operandNode->GetType() == NodeType::ATOMIC)
            {
                const Token &operandToken = static_cast<Atomic *>(operandNode.get())->GetToken();

                if (operandToken.GetType() == TokenType::OPERATOR &&
                    operators.find(currentNodeToken.GetValue<String>()) != operators.end())
                {
                    outNodes.push_back(currentNode);
                    sourceNodeIndex++;
                    continue;
                }
            }
            else if (operandNode->GetType() == NodeType::EXPRESSION)
            {
            }
            else if (operandNode->GetType() == NodeType::UNARY_OPERATION)
            {
            }
            else if (operandNode->GetType() == NodeType::BLOCK)
            {
                Ref<Node> newUnaryNode = CreateRef<UnaryOperationNode>(
                    currentNode, CreateRef<InvalidNode>());
                newUnaryNode->AddError(ErrorType::MISSING_RIGHT_OPERAND);
                outNodes.push_back(newUnaryNode);
                sourceNodeIndex++;
                continue;
            }
            else
            {
                outNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            Ref<Node> newUnaryNode = CreateRef<UnaryOperationNode>(currentNode, operandNode);
            outNodes.push_back(newUnaryNode);
            foundUnary = NTT_TRUE;
            sourceNodeIndex += 2;
        }
    }

    void ParseOperations(const Vector<Ref<Node>> &sourceNodes,
                         Vector<Ref<Node>> &outNodes,
                         b8 &foundUnary,
                         const std::set<String> &operators)
    {
        NTT_ASSERT(outNodes.empty());
        u32 numberOfSourceNodes = u32(sourceNodes.size());
        u32 sourceNodeIndex = 0;

        while (sourceNodeIndex < numberOfSourceNodes)
        {
            const Ref<Node> &currentNode = sourceNodes[sourceNodeIndex];

            if (currentNode->GetType() != NodeType::ATOMIC)
            {
                outNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            Atomic *atomicNode = dynamic_cast<Atomic *>(currentNode.get());
            const Token &currentNodeToken = atomicNode->GetToken();

            if (currentNodeToken.GetType() != TokenType::OPERATOR ||
                operators.find(currentNodeToken.GetValue<String>()) == operators.end())
            {
                outNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            if (sourceNodeIndex + 1 >= numberOfSourceNodes)
            {
                Ref<Node> newUnaryNode = CreateRef<OperationNode>(
                    currentNode, CreateRef<InvalidNode>(), CreateRef<InvalidNode>());
                newUnaryNode->AddError(ErrorType::MISSING_RIGHT_OPERAND);
                outNodes.push_back(newUnaryNode);
                sourceNodeIndex++;
                continue;
            }

            const Ref<Node> &rightOperandNode = sourceNodes[sourceNodeIndex + 1];
            const Ref<Node> &leftOperandNode = sourceNodes[sourceNodeIndex - 1];

            Ref<Node> newUnaryNode = CreateRef<OperationNode>(currentNode, leftOperandNode, rightOperandNode);
            outNodes.pop_back();
            outNodes.push_back(newUnaryNode);
            foundUnary = NTT_TRUE;
            sourceNodeIndex += 2;
        }
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