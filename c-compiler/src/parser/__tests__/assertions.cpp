#include "test_common.h"
#include "assertions.h"
#include "parser/operationNode.h"
#include "parser/if_statement.h"

#define DEFINE_ERROR_BUFFER()                      \
    char errorBuffer[10000];                       \
    memset(errorBuffer, 0, sizeof((errorBuffer))); \
    sprintf_s(errorBuffer, "\nNode (): %s\n", node->ToJSON().dump(4).c_str());

void DelayAssertion::Assert(Ref<Node> node)
{
    _Assert(node);

    if (HasError())
    {
        EXPECT_THAT(node->HasErrors(), true);
        EXPECT_THAT(node->GetErrors(),
                    ::testing::Contains(static_cast<ErrorType>(GetError())));
    }
}

void BlockAssertion::_Assert(Ref<Node> node)
{
    DEFINE_ERROR_BUFFER();

    EXPECT_THAT(node->GetType(), m_blockType);

    BlockNode *blockNode = dynamic_cast<BlockNode *>(node.get());

    EXPECT_THAT(blockNode->GetChildren().size(), m_assertions.size()) << errorBuffer;
    u32 numberOfAssertions = u32(m_assertions.size());

    for (u32 i = 0; i < numberOfAssertions; i++)
    {
        m_assertions[i]->Assert(blockNode->GetChildren()[i]);
    }
}

void AtomicAssertion::_Assert(Ref<Node> node)
{
    DEFINE_ERROR_BUFFER();

    EXPECT_THAT(node->GetType(), NodeType::ATOMIC) << errorBuffer;

    Atomic *atomicNode = dynamic_cast<Atomic *>(node.get());
    EXPECT_THAT(atomicNode != nullptr, true) << errorBuffer;

    const Token &token = atomicNode->GetToken();
    EXPECT_THAT(token.GetType(), m_expectType) << errorBuffer;

    switch (m_expectType)
    {
    case TokenType::INTEGER:
        EXPECT_THAT(token.GetValue<u32>(), m_expectValue.numberValue.intValue) << errorBuffer;
        break;
    case TokenType::FLOAT:
        EXPECT_THAT(token.GetValue<f32>(), m_expectValue.numberValue.floatValue) << errorBuffer;
        break;
    case TokenType::BOOLEAN:
        EXPECT_THAT(token.GetValue<b8>(), m_expectValue.numberValue.boolValue) << errorBuffer;
        break;
    case TokenType::STRING:
    case TokenType::INVALID:
    case TokenType::KEYWORD:
    case TokenType::BRACKET:
    case TokenType::DELIMITER:
    case TokenType::IDENTIFIER:
    case TokenType::OPERATOR:
        EXPECT_THAT(token.GetValue<std::string>(), m_expectValue.stringValue) << errorBuffer;
        break;
    default:
        // Unknown type
        NTT_ASSERT_MSG(false, "Unknown token type in assertion.");
        break;
    }
}

void InvalidAssertion::_Assert(Ref<Node> node)
{
    DEFINE_ERROR_BUFFER();

    EXPECT_THAT(node->GetType(), NodeType::INVALID) << errorBuffer;

    if (HasError())
    {
        EXPECT_THAT(node->GetErrors(),
                    ::testing::Contains(static_cast<ErrorType>(GetError())))
            << errorBuffer;
    }
}

void UnaryOperationAssertion::_Assert(Ref<Node> node)
{
    DEFINE_ERROR_BUFFER();

    EXPECT_THAT(node->GetType(), NodeType::UNARY_OPERATION) << errorBuffer;

    UnaryOperationNode *unaryNode = dynamic_cast<UnaryOperationNode *>(node.get());
    EXPECT_THAT(unaryNode != nullptr, true) << errorBuffer;

    m_operand->Assert(unaryNode->GetOperand());
    m_operation->Assert(unaryNode->GetOperator());
}

void OperationAssertion::_Assert(Ref<Node> node)
{
    DEFINE_ERROR_BUFFER();

    EXPECT_THAT(node->GetType(), NodeType::OPERATION) << errorBuffer;

    OperationNode *expressionNode = dynamic_cast<OperationNode *>(node.get());
    EXPECT_THAT(expressionNode != nullptr, true) << errorBuffer;

    m_leftOperand->Assert(expressionNode->GetLeftOperand());
    m_operatorNode->Assert(expressionNode->GetOperator());
    m_rightOperand->Assert(expressionNode->GetRightOperand());
}

void IfStatementAssertion::_Assert(Ref<Node> node)
{
    DEFINE_ERROR_BUFFER();

    EXPECT_THAT(node->GetType(), NodeType::IF_STATEMENT) << errorBuffer;

    IfStatementNode *ifNode = dynamic_cast<IfStatementNode *>(node.get());
    EXPECT_THAT(ifNode != nullptr, true) << errorBuffer;

    m_condition->Assert(ifNode->GetCondition());
    m_block->Assert(ifNode->GetBlock());
    m_elseBlock->Assert(ifNode->GetElseBlock());
}