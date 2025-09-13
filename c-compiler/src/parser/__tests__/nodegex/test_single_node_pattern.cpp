#include "test_common.h"
#include "parser/nodegex/single_node_pattern.h"
#include "tokenizer/tokenType.h"
#include "parser/atomic.h"

using namespace ntt;

#define CREATE_ATOMIC_NODE(name, type, value) \
    Token token##name(type, 0);               \
    token##name.SetValue(value);              \
    Ref<Node> name = CreateRef<Atomic>(NodeType::ATOMIC, token##name);

TEST(SingleNodePatternTest, GetPatternLength)
{
    NodePatternPair pair;
    pair.type = NodeType::EXPRESSION;
    pair.values.push_back(TokenValue{});

    SingleNodePattern pattern({pair});
    EXPECT_EQ(pattern.GetPatternLength(), 1);
}

TEST(SingleNodePatternTest, MatchAtomicNode)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));

    NodePatternPair pair;
    pair.type = NodeType::ATOMIC;

    Ref<NodePattern> pattern = CreateRef<SingleNodePattern>(pair);
    EXPECT_TRUE(pattern->Match(Vector<Ref<Node>>{atomicNode}, 0));
}

TEST(SingleNodePatternTest, NotMatchAtomicNode)
{
    CREATE_ATOMIC_NODE(atomicNode, TokenType::IDENTIFIER, String("foo"));

    NodePatternPair pair;
    pair.type = NodeType::EXPRESSION;

    Ref<NodePattern> pattern = CreateRef<SingleNodePattern>(pair);
    EXPECT_FALSE(pattern->Match(Vector<Ref<Node>>{atomicNode}, 0));
}