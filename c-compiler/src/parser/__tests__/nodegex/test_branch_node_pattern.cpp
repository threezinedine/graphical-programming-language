#include "nodegex_common.h"
#include "parser/nodegex/branch_pattern.h"

TEST(BranchPatternTest, MatchBranch)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    CREATE_ATOMIC_NODE(atomicNode1, TokenType::BOOLEAN, false);
    CREATE_ATOMIC_NODE(atomicNode2, TokenType::IDENTIFIER, String("test"));

    Ref<NodePattern> pattern = CreateRef<BranchPatternBuilder>()
                                   ->AddPattern(
                                       CreateRef<SingleNodePatternBuilder>(NodeType::ATOMIC)
                                           ->AddValue(TokenType::IDENTIFIER, String("foo")))
                                   ->AddPattern(
                                       CreateRef<SingleNodePatternBuilder>(NodeType::ATOMIC)
                                           ->AddValue(TokenType::BOOLEAN, false))
                                   ->Build();

    u32 startIndex = 0;
    EXPECT_TRUE(pattern->Match(Vector<Ref<Node>>{atomicNode, atomicNode1, atomicNode2}, startIndex));
    EXPECT_EQ(startIndex, 1);
}

TEST(BranchPatternTest, NotMatchBranch)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));
    CREATE_ATOMIC_NODE(atomicNode1, TokenType::BOOLEAN, false);
    CREATE_ATOMIC_NODE(atomicNode2, TokenType::IDENTIFIER, String("test"));

    Ref<NodePattern> pattern = CreateRef<BranchPatternBuilder>()
                                   ->AddPattern(
                                       CreateRef<SingleNodePatternBuilder>(NodeType::BLOCK))
                                   ->AddPattern(
                                       CreateRef<SingleNodePatternBuilder>(NodeType::EXPRESSION))
                                   ->Build();

    u32 startIndex = 0;
    EXPECT_FALSE(pattern->Match(Vector<Ref<Node>>{atomicNode, atomicNode1, atomicNode2}, startIndex));
    EXPECT_EQ(startIndex, 0);
}
