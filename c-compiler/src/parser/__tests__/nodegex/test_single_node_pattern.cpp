#include "test_common.h"
#include "nodegex_common.h"

TEST(SingleNodePatternTest, GetPatternLength)
{
    DEFINE_SINGLE_PATTERN(pattern, NodeType::ATOMIC);

    EXPECT_EQ(pattern.GetPatternLength(), 1);
}

TEST(SingleNodePatternTest, MatchAtomicNode)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    DEFINE_SINGLE_PATTERN(pattern, NodeType::ATOMIC);

    EXPECT_TRUE(pattern.Match(Vector<Ref<Node>>{atomicNode}, 0));
}

TEST(SingleNodePatternTest, NotMatchAtomicNode)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    DEFINE_SINGLE_PATTERN(pattern, NodeType::EXPRESSION);

    EXPECT_FALSE(pattern.Match(Vector<Ref<Node>>{atomicNode}, 0));
}

TEST(SingleNodePatternTest, DoNotMatchValue)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    DEFINE_SINGLE_PATTERN_1(pattern, NodeType::ATOMIC, "bar");

    EXPECT_FALSE(pattern.Match(Vector<Ref<Node>>{atomicNode}, 0));
}

TEST(SingleNodePatternTest, MatchValueInMultipleOptions)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    DEFINE_SINGLE_PATTERN_2(pattern, NodeType::ATOMIC, "foo", "bar");

    EXPECT_TRUE(pattern.Match(Vector<Ref<Node>>{atomicNode}, 0));
}

TEST(SingleNodePatternTest, NotMatchInMultipleOptions)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    DEFINE_SINGLE_PATTERN_2(pattern, NodeType::ATOMIC, "bar", "baz");

    EXPECT_FALSE(pattern.Match(Vector<Ref<Node>>{atomicNode}, 0));
}

TEST(SingleNodePatternTest, CheckMatchIfIndexOutOfBounds)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    DEFINE_SINGLE_PATTERN(pattern, NodeType::ATOMIC);

    EXPECT_FALSE(pattern.Match(Vector<Ref<Node>>{atomicNode}, 1));
}