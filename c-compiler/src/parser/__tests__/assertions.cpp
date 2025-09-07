#include "test_common.h"
#include "assertions.h"

void BlockAssertion::Assert(Ref<Node> node)
{
    EXPECT_THAT(node->GetType(), m_blockType);

    BlockNode *blockNode = dynamic_cast<BlockNode *>(node.get());

    EXPECT_THAT(blockNode->GetChildren().size(), m_assertions.size());
    u32 numberOfAssertions = u32(m_assertions.size());

    for (u32 i = 0; i < numberOfAssertions; i++)
    {
        m_assertions[i]->Assert(blockNode->GetChildren()[i]);
    }

    if (HasError())
    {
        EXPECT_THAT(node->GetErrors(),
                    ::testing::Contains(static_cast<ErrorType>(GetError())));
    }
}

void AtomicAssertion::Assert(Ref<Node> node)
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