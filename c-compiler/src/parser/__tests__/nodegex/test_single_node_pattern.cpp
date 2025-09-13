#include "test_common.h"
#include "parser/nodegex/single_node_pattern.h"
#include "tokenizer/tokenType.h"
#include "parser/atomic.h"

using namespace ntt;

#define CREATE_ATOMIC_NODE(name, type, value) \
    Token token##name(type, 0);               \
    token##name.SetValue(value);              \
    Ref<Node> name = CreateRef<Atomic>(NodeType::ATOMIC, token##name);

template <typename T>
void AddTokenValue(NodePatternPair &pair, T value);

template <>
void AddTokenValue<u32>(NodePatternPair &pair, u32 value)
{
    Token token(TokenType::INTEGER, 0);
    token.SetValue(value);
    pair.values.push_back(token);
}

template <>
void AddTokenValue<String>(NodePatternPair &pair, String value)
{
    Token token(TokenType::IDENTIFIER, 0);
    token.SetValue(value);
    pair.values.push_back(token);
}

template <>
void AddTokenValue<const char *>(NodePatternPair &pair, const char *value)
{
    AddTokenValue<String>(pair, String(value));
}

template <>
void AddTokenValue<b8>(NodePatternPair &pair, b8 value)
{
    Token token(TokenType::BOOLEAN, 0);
    token.SetValue(value);
    pair.values.push_back(token);
}

template <>
void AddTokenValue<f32>(NodePatternPair &pair, f32 value)
{
    Token token(TokenType::FLOAT, 0);
    token.SetValue(value);
    pair.values.push_back(token);
}

#define DEFINE_SINGLE_PATTERN(name, tokenType, ...) \
    NodePatternPair pair;                           \
    pair.type = tokenType;                          \
    SingleNodePattern name(pair);

#define DEFINE_SINGLE_PATTERN_1(name, tokenType, value1) \
    NodePatternPair pair;                                \
    pair.type = tokenType;                               \
    AddTokenValue(pair, value1);                         \
    SingleNodePattern name(pair);

#define DEFINE_SINGLE_PATTERN_2(name, tokenType, value1, value2) \
    NodePatternPair pair;                                        \
    pair.type = tokenType;                                       \
    AddTokenValue(pair, value1);                                 \
    AddTokenValue(pair, value2);                                 \
    SingleNodePattern name(pair);

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