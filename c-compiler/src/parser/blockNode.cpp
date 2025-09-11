#include "parser/blockNode.h"
#include "tokenizer/tokenizer.h"
#include "parser/atomic.h"
#include <set>
#include "parser/unaryOperationNode.h"
#include "parser/invalid.h"
#include "parser/operationNode.h"
#include "parser/if_statement.h"
#include "parser/function_call.h"

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
        json["type"] = NodeTypeToString(m_type);
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
                                     b8 &hasAnyChange,
                                     const std::set<String> &operators);

    static void ParseOperations(const Vector<Ref<Node>> &sourceNodes,
                                Vector<Ref<Node>> &outNodes,
                                b8 &hasAnyChange,
                                const std::set<String> &operators);

    static void ParseStatements(const Vector<Ref<Node>> &sourceNodes,
                                Vector<Ref<Node>> &outNodes);

    static void ParseIfStatements(const Vector<Ref<Node>> &sourceNodes,
                                  Vector<Ref<Node>> &outNodes,
                                  b8 &hasAnyChange);

    static void ParseFunctionCall(const Vector<Ref<Node>> &sourceNodes,
                                  Vector<Ref<Node>> &outNodes,
                                  b8 &hasAnyChange);

    static void ParseExpressions(const Vector<Ref<Node>> &sourceNodes,
                                 Vector<Ref<Node>> &outNodes, b8 &containsComma);

    void BlockNode::Parse()
    {
        Vector<Ref<Node>> parsedNodes = m_children;
        b8 hasAnyModified = NTT_TRUE;

        for (auto &node : m_children)
        {
            node->Parse();
        }

        if (GetType() == NodeType::BLOCK || GetType() == NodeType::PROGRAM)
        {
            Vector<Ref<Node>> newParsedNodes;

            while (hasAnyModified)
            {
                Vector<Ref<Node>> newParsedNodes;
                hasAnyModified = NTT_FALSE;

                ParseIfStatements(
                    parsedNodes,
                    newParsedNodes,
                    hasAnyModified);

                parsedNodes = newParsedNodes;
            }

            hasAnyModified = NTT_TRUE;

            ParseStatements(
                parsedNodes,
                newParsedNodes);

            m_children = newParsedNodes;

            for (auto &node : m_children)
            {
                node->Parse();
            }

            return;
        }

        hasAnyModified = NTT_TRUE;

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
                std::set<String>{"==", "!=", "<", "<=", ">", ">="});
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
                std::set<String>{"^"});
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
                std::set<String>{"*", "/"});
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
                std::set<String>{"+", "-"});
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
                std::set<String>{"="});
            parsedNodes = newParsedNodes;
        }

        hasAnyModified = NTT_TRUE;
        while (hasAnyModified)
        {
            hasAnyModified = NTT_FALSE;
            Vector<Ref<Node>> newerParsedNodes;
            ParseFunctionCall(
                parsedNodes,
                newerParsedNodes,
                hasAnyModified);

            parsedNodes = newerParsedNodes;
        }

        if (GetType() == NodeType::EXPRESSION)
        {
            bool containsComma = NTT_FALSE;
            Vector<Ref<Node>> newParsedNodes;
            ParseExpressions(
                parsedNodes,
                newParsedNodes,
                containsComma);

            if (containsComma)
            {
                parsedNodes = newParsedNodes;
            }
        }

        m_children = parsedNodes;
    }

    static b8 IsOperandValidNode(const Ref<Node> &node)
    {
        if (node->GetType() != NodeType::ATOMIC &&
            node->GetType() != NodeType::EXPRESSION &&
            node->GetType() != NodeType::UNARY_OPERATION &&
            node->GetType() != NodeType::OPERATION)
        {
            return NTT_FALSE;
        }

        if (node->GetType() == NodeType::ATOMIC)
        {
            const Token &token = static_cast<Atomic *>(node.get())->GetToken();
            if (token.GetType() == TokenType::BOOLEAN ||
                token.GetType() == TokenType::FLOAT ||
                token.GetType() == TokenType::INTEGER ||
                token.GetType() == TokenType::STRING ||
                token.GetType() == TokenType::IDENTIFIER)
            {
                return NTT_TRUE;
            }
            else
            {
                return NTT_FALSE;
            }
        }

        return NTT_TRUE;
    }

    void ParseUnaryOperations(const Vector<Ref<Node>> &sourceNodes,
                              Vector<Ref<Node>> &outNodes,
                              b8 &hasAnyChange,
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
            hasAnyChange = NTT_TRUE;
            sourceNodeIndex += 2;
        }
    }

    void ParseOperations(const Vector<Ref<Node>> &sourceNodes,
                         Vector<Ref<Node>> &outNodes,
                         b8 &hasAnyChange,
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

            Ref<Node> leftOperandNode = NTT_NULL;
            Ref<Node> rightOperandNode = NTT_NULL;
            u8 moveIndexSteps = 0;
            Vector<ErrorType> errors;

            if (outNodes.size() > 0)
            {
                leftOperandNode = outNodes.back();
                outNodes.pop_back();
            }
            else
            {
                leftOperandNode = CreateRef<InvalidNode>();
                errors.push_back(ErrorType::MISSING_LEFT_OPERAND);
            }

            if (sourceNodeIndex + 1 < numberOfSourceNodes &&
                IsOperandValidNode(sourceNodes[sourceNodeIndex + 1]))
            {
                rightOperandNode = sourceNodes[sourceNodeIndex + 1];
                moveIndexSteps += 2;
            }
            else
            {
                rightOperandNode = CreateRef<InvalidNode>();
                errors.push_back(ErrorType::MISSING_RIGHT_OPERAND);
                moveIndexSteps += 1;
            }

            Ref<Node> newOperation = CreateRef<OperationNode>(currentNode, leftOperandNode, rightOperandNode);

            for (const auto &error : errors)
            {
                newOperation->AddError(error);
            }

            outNodes.push_back(newOperation);
            hasAnyChange = NTT_TRUE;
            sourceNodeIndex += moveIndexSteps;
        }
    }

    static void ParseStatements(const Vector<Ref<Node>> &sourceNodes,
                                Vector<Ref<Node>> &outNodes)
    {
        NTT_ASSERT(outNodes.empty());
        u32 numberOfSourceNodes = u32(sourceNodes.size());
        u32 sourceNodeIndex = 0;

        Vector<Ref<Node>> currentStatementNodes;

        while (sourceNodeIndex < numberOfSourceNodes)
        {
            const Ref<Node> &currentNode = sourceNodes[sourceNodeIndex];

            if (currentNode->GetType() != NodeType::ATOMIC &&
                currentNode->GetType() != NodeType::OPERATION &&
                currentNode->GetType() != NodeType::UNARY_OPERATION &&
                currentNode->GetType() != NodeType::EXPRESSION)
            {
                if (currentStatementNodes.size() != 0)
                {
                    Ref<Node> newTemporaryBlock = CreateRef<BlockNode>(NodeType::STATEMENT, currentStatementNodes);
                    outNodes.push_back(newTemporaryBlock);
                    currentStatementNodes.clear();
                }

                outNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            if (currentNode->GetType() == NodeType::OPERATION ||
                currentNode->GetType() == NodeType::UNARY_OPERATION ||
                currentNode->GetType() == NodeType::EXPRESSION)
            {
                currentStatementNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            Atomic *atomicNode = dynamic_cast<Atomic *>(currentNode.get());
            const Token &currentNodeToken = atomicNode->GetToken();

            if (currentNodeToken.GetType() != TokenType::DELIMITER ||
                currentNodeToken.GetValue<String>() != ";")
            {
                currentStatementNodes.push_back(currentNode);
            }
            else
            {
                if (currentStatementNodes.size() != 0)
                {
                    Ref<Node> newTemporaryBlock = CreateRef<BlockNode>(NodeType::STATEMENT, currentStatementNodes);
                    outNodes.push_back(newTemporaryBlock);
                    currentStatementNodes.clear();
                }
            }

            sourceNodeIndex++;
        }

        if (currentStatementNodes.size() != 0)
        {
            Ref<Node> newTemporaryBlock = CreateRef<BlockNode>(NodeType::STATEMENT, currentStatementNodes);
            newTemporaryBlock->AddError(ErrorType::MISSING_SEMICOLON);
            outNodes.push_back(newTemporaryBlock);
            currentStatementNodes.clear();
        }
    }

    static void ParseIfStatements(const Vector<Ref<Node>> &sourceNodes,
                                  Vector<Ref<Node>> &outNodes, b8 &hasAnyChange)
    {
        NTT_ASSERT(outNodes.empty());
        u32 numberOfSourceNodes = u32(sourceNodes.size());
        u32 sourceNodeIndex = 0;

        while (sourceNodeIndex < numberOfSourceNodes)
        {
            Ref<Node> currentNode = sourceNodes[sourceNodeIndex];

            if (currentNode->GetType() != NodeType::ATOMIC)
            {
                outNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            Atomic *atomicNode = dynamic_cast<Atomic *>(currentNode.get());
            const Token &currentNodeToken = atomicNode->GetToken();

            if (currentNodeToken.GetType() != TokenType::KEYWORD ||
                currentNodeToken.GetValue<String>() != "if")
            {
                outNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            Ref<Node> conditionNode = NTT_NULL;
            Ref<Node> blockNode = CreateRef<BlockNode>(NodeType::BLOCK, Vector<Ref<Node>>{});
            Ref<Node> elseNode = NTT_NULL;
            Ref<Node> elseBlockNode = CreateRef<BlockNode>(NodeType::BLOCK, Vector<Ref<Node>>{});
            Vector<ErrorType> errors;

            u32 tempIndex = sourceNodeIndex + 1;

            if (tempIndex < numberOfSourceNodes &&
                sourceNodes[tempIndex]->GetType() == NodeType::EXPRESSION)
            {
                conditionNode = sourceNodes[tempIndex];
                tempIndex++;
            }
            else
            {
                conditionNode = CreateRef<InvalidNode>();
                errors.push_back(ErrorType::MISSING_CONDITION);
            }

            if (tempIndex < numberOfSourceNodes &&
                sourceNodes[tempIndex]->GetType() == NodeType::BLOCK)
            {
                blockNode = sourceNodes[tempIndex];
                tempIndex++;
            }
            else
            {
                errors.push_back(ErrorType::MISSING_BLOCK);
            }

            if (tempIndex < numberOfSourceNodes &&
                sourceNodes[tempIndex]->GetType() == NodeType::ATOMIC)
            {
                Atomic *elseAtomicNode = dynamic_cast<Atomic *>(sourceNodes[tempIndex].get());
                const Token &elseToken = elseAtomicNode->GetToken();

                if (elseToken.GetType() == TokenType::KEYWORD &&
                    elseToken.GetValue<String>() == "else")
                {
                    tempIndex++;
                    elseNode = sourceNodes[tempIndex];

                    if (tempIndex < numberOfSourceNodes)
                    {
                        if (sourceNodes[tempIndex]->GetType() == NodeType::BLOCK ||
                            sourceNodes[tempIndex]->GetType() == NodeType::IF_STATEMENT)
                        {
                            elseBlockNode = sourceNodes[tempIndex];
                            tempIndex++;
                        }
                        else if (sourceNodes[tempIndex]->GetType() == NodeType::ATOMIC &&
                                 dynamic_cast<Atomic *>(sourceNodes[tempIndex].get())->GetToken().GetType() == TokenType::KEYWORD &&
                                 dynamic_cast<Atomic *>(sourceNodes[tempIndex].get())->GetToken().GetValue<String>() == "if")
                        {
                            for (u32 i = sourceNodeIndex; i < tempIndex; i++)
                            {
                                outNodes.push_back(sourceNodes[i]);
                            }

                            sourceNodeIndex = tempIndex;
                            continue;
                        }
                    }
                }
            }

            Ref<Node> newIfNode = CreateRef<IfStatementNode>(conditionNode, blockNode, elseBlockNode);
            for (const auto &error : errors)
            {
                newIfNode->AddError(error);
            }
            outNodes.push_back(newIfNode);
            sourceNodeIndex = tempIndex;
            hasAnyChange = NTT_TRUE;
        }
    }

    static void ParseFunctionCall(const Vector<Ref<Node>> &sourceNodes,
                                  Vector<Ref<Node>> &outNodes,
                                  b8 &hasAnyChange)
    {
        NTT_ASSERT(outNodes.empty());
        u32 numberOfSourceNodes = u32(sourceNodes.size());
        u32 sourceNodeIndex = 0;

        while (sourceNodeIndex < numberOfSourceNodes)
        {
            u32 temporaryIndex = sourceNodeIndex;

            Ref<Node> currentNode = sourceNodes[sourceNodeIndex];
            if (currentNode->GetType() != NodeType::ATOMIC)
            {
                outNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            Atomic *atomicNode = dynamic_cast<Atomic *>(currentNode.get());
            const Token &currentNodeToken = atomicNode->GetToken();

            if (currentNodeToken.GetType() != TokenType::IDENTIFIER)
            {
                outNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            Ref<Node> functionCallNode = CreateRef<FunctionCallNode>(currentNode, Vector<Ref<Node>>{});
            temporaryIndex++;

            if (temporaryIndex >= numberOfSourceNodes ||
                sourceNodes[temporaryIndex]->GetType() != NodeType::EXPRESSION)
            {
                outNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            temporaryIndex++;

            outNodes.push_back(functionCallNode);
            sourceNodeIndex = temporaryIndex;
            hasAnyChange = NTT_TRUE;
        }
    }

    static void ParseExpressions(const Vector<Ref<Node>> &sourceNodes,
                                 Vector<Ref<Node>> &outNodes, b8 &containsComma)
    {
        NTT_ASSERT(outNodes.empty());
        u32 numberOfSourceNodes = u32(sourceNodes.size());
        u32 sourceNodeIndex = 0;

        if (numberOfSourceNodes < 2)
        {
            outNodes = sourceNodes;
            return;
        }

        Vector<Ref<Node>> temporaryNodes;

        while (sourceNodeIndex < numberOfSourceNodes)
        {
            Ref<Node> currentNode = sourceNodes[sourceNodeIndex];

            if (currentNode->GetType() != NodeType::ATOMIC)
            {
                temporaryNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            Atomic *atomicNode = dynamic_cast<Atomic *>(currentNode.get());
            const Token &currentNodeToken = atomicNode->GetToken();

            if (currentNodeToken.GetType() != TokenType::DELIMITER ||
                currentNodeToken.GetValue<String>() != ",")
            {
                temporaryNodes.push_back(currentNode);
                sourceNodeIndex++;
                continue;
            }

            containsComma = NTT_TRUE;

            if (temporaryNodes.size() == 1)
            {
                outNodes.push_back(temporaryNodes[0]);
                temporaryNodes.clear();
                sourceNodeIndex++;
                continue;
            }

            Ref<Node> newExpressionNode = CreateRef<BlockNode>(NodeType::EXPRESSION, temporaryNodes);
            outNodes.push_back(newExpressionNode);
            temporaryNodes.clear();
            sourceNodeIndex++;
        }

        if (temporaryNodes.size() == 1)
        {
            outNodes.push_back(temporaryNodes[0]);
            temporaryNodes.clear();
        }
        else if (temporaryNodes.size() > 1)
        {
            Ref<Node> newExpressionNode = CreateRef<BlockNode>(NodeType::EXPRESSION, temporaryNodes);
            outNodes.push_back(newExpressionNode);
            temporaryNodes.clear();
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