#include "test_common.h"
#include "nodegex_common.h"

TEST(SingleNodePatternTest, MatchAtomicNode)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    Ref<NodePattern> pattern = SingleNodePatternBuilder(NodeType::ATOMIC).Build();

    u32 startIndex = 0;
    EXPECT_TRUE(pattern->Match(Vector<Ref<Node>>{atomicNode}, startIndex));
    EXPECT_EQ(startIndex, 1);
}

TEST(SingleNodePatternTest, NotMatchAtomicNode)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    Ref<NodePattern> pattern = SingleNodePatternBuilder(NodeType::EXPRESSION).Build();

    u32 startIndex = 0;
    EXPECT_FALSE(pattern->Match(Vector<Ref<Node>>{atomicNode}, startIndex));
    EXPECT_EQ(startIndex, 0);
}

TEST(SingleNodePatternTest, DoNotMatchValue)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    Ref<NodePattern> pattern = SingleNodePatternBuilder(NodeType::ATOMIC)
                                   .AddValue(TokenType::IDENTIFIER, String("bar"))
                                   .Build();

    u32 startIndex = 0;
    EXPECT_FALSE(pattern->Match(Vector<Ref<Node>>{atomicNode}, startIndex));
}

TEST(SingleNodePatternTest, MatchValueInMultipleOptions)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    Ref<NodePattern> pattern = SingleNodePatternBuilder(NodeType::ATOMIC)
                                   .AddValue(TokenType::IDENTIFIER, String("bar"))
                                   .AddValue(TokenType::IDENTIFIER, String("foo"))
                                   .Build();

    u32 startIndex = 0;
    EXPECT_TRUE(pattern->Match(Vector<Ref<Node>>{atomicNode}, startIndex));
    EXPECT_EQ(startIndex, 1);
}

TEST(SingleNodePatternTest, NotMatchInMultipleOptions)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    Ref<NodePattern> pattern = SingleNodePatternBuilder(NodeType::ATOMIC)
                                   .AddValue(TokenType::IDENTIFIER, String("bar"))
                                   .AddValue(TokenType::IDENTIFIER, String("baz"))
                                   .Build();

    u32 startIndex = 0;
    EXPECT_FALSE(pattern->Match(Vector<Ref<Node>>{atomicNode}, startIndex));
    EXPECT_EQ(startIndex, 0);
}

TEST(SingleNodePatternTest, CheckMatchIfIndexOutOfBounds)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    Ref<NodePattern> pattern = SingleNodePatternBuilder(NodeType::ATOMIC).Build();

    u32 startIndex = 1;
    EXPECT_FALSE(pattern->Match(Vector<Ref<Node>>{atomicNode}, startIndex));
    EXPECT_EQ(startIndex, 1);
}