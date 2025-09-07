#include "test_common.h"
#include "assertions.h"

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
    EXPECT_THAT(node->GetType(), m_blockType);

    BlockNode *blockNode = dynamic_cast<BlockNode *>(node.get());

    EXPECT_THAT(blockNode->GetChildren().size(), m_assertions.size());
    u32 numberOfAssertions = u32(m_assertions.size());

    for (u32 i = 0; i < numberOfAssertions; i++)
    {
        m_assertions[i]->Assert(blockNode->GetChildren()[i]);
    }
}

void AtomicAssertion::_Assert(Ref<Node> node)
{
    EXPECT_THAT(node->GetType(), NodeType::ATOMIC);

    Atomic *atomicNode = dynamic_cast<Atomic *>(node.get());
    EXPECT_THAT(atomicNode != nullptr, true);

    const Token &token = atomicNode->GetToken();
    EXPECT_THAT(token.GetType(), m_expectType);

    switch (m_expectType)
    {
    case TokenType::INTEGER:
        EXPECT_THAT(token.GetValue<u32>(), m_expectValue.numberValue.intValue);
        break;
    case TokenType::FLOAT:
        EXPECT_THAT(token.GetValue<f32>(), m_expectValue.numberValue.floatValue);
        break;
    case TokenType::BOOLEAN:
        EXPECT_THAT(token.GetValue<b8>(), m_expectValue.numberValue.boolValue);
        break;
    case TokenType::STRING:
    case TokenType::INVALID:
    case TokenType::KEYWORD:
    case TokenType::BRACKET:
    case TokenType::DELIMITER:
    case TokenType::IDENTIFIER:
    case TokenType::OPERATOR:
        EXPECT_THAT(token.GetValue<std::string>(), m_expectValue.stringValue);
        break;
    default:
        // Unknown type
        NTT_ASSERT_MSG(false, "Unknown token type in assertion.");
        break;
    }
}

void InvalidAssertion::_Assert(Ref<Node> node)
{
    EXPECT_THAT(node->GetType(), NodeType::INVALID);

    if (HasError())
    {
        EXPECT_THAT(node->GetErrors(),
                    ::testing::Contains(static_cast<ErrorType>(GetError())));
    }
}

void UnaryOperationAssertion::_Assert(Ref<Node> node)
{
    EXPECT_THAT(node->GetType(), NodeType::UNARY_OPERATION);

    UnaryOperationNode *unaryNode = dynamic_cast<UnaryOperationNode *>(node.get());
    EXPECT_THAT(unaryNode != nullptr, true);

    m_operand->Assert(unaryNode->GetOperand());
    m_operation->Assert(unaryNode->GetOperator());

    if (HasError())
    {
        EXPECT_THAT(node->GetErrors(),
                    ::testing::Contains(static_cast<ErrorType>(GetError())));
    }
}