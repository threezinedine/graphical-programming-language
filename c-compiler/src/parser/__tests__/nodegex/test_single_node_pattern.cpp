#include "test_common.h"
#include "nodegex_common.h"

TEST(SingleNodePatternTest, GetPatternLength)
{
    Ref<NodePattern> pattern = SingleNodePatternBuilder(NodeType::ATOMIC).Build();
    EXPECT_EQ(pattern->GetPatternLength(), 1);
}

TEST(SingleNodePatternTest, MatchAtomicNode)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    Ref<NodePattern> pattern = SingleNodePatternBuilder(NodeType::ATOMIC).Build();

    EXPECT_TRUE(pattern->Match(Vector<Ref<Node>>{atomicNode}, 0));
}

TEST(SingleNodePatternTest, NotMatchAtomicNode)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    Ref<NodePattern> pattern = SingleNodePatternBuilder(NodeType::EXPRESSION).Build();

    EXPECT_FALSE(pattern->Match(Vector<Ref<Node>>{atomicNode}, 0));
}

TEST(SingleNodePatternTest, DoNotMatchValue)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    Ref<NodePattern> pattern = SingleNodePatternBuilder(NodeType::ATOMIC)
                                   .AddValue(TokenType::IDENTIFIER, String("bar"))
                                   .Build();

    EXPECT_FALSE(pattern->Match(Vector<Ref<Node>>{atomicNode}, 0));
}

TEST(SingleNodePatternTest, MatchValueInMultipleOptions)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    Ref<NodePattern> pattern = SingleNodePatternBuilder(NodeType::ATOMIC)
                                   .AddValue(TokenType::IDENTIFIER, String("bar"))
                                   .AddValue(TokenType::IDENTIFIER, String("foo"))
                                   .Build();

    EXPECT_TRUE(pattern->Match(Vector<Ref<Node>>{atomicNode}, 0));
}

TEST(SingleNodePatternTest, NotMatchInMultipleOptions)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    Ref<NodePattern> pattern = SingleNodePatternBuilder(NodeType::ATOMIC)
                                   .AddValue(TokenType::IDENTIFIER, String("bar"))
                                   .AddValue(TokenType::IDENTIFIER, String("baz"))
                                   .Build();

    EXPECT_FALSE(pattern->Match(Vector<Ref<Node>>{atomicNode}, 0));
}

TEST(SingleNodePatternTest, CheckMatchIfIndexOutOfBounds)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    Ref<NodePattern> pattern = SingleNodePatternBuilder(NodeType::ATOMIC).Build();

    EXPECT_FALSE(pattern->Match(Vector<Ref<Node>>{atomicNode}, 1));
}