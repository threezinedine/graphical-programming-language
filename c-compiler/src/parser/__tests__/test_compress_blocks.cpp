#include "test_common.h"
#include "parser/blockNode.h"
#include "assertions.h"

using namespace ntt;

TEST(BlockNodeCompressTest, CreateWithContent)
{
    BlockNode blockNode(NodeType::PROGRAM, "23");
    blockNode.Compress();

    CreateRef<BlockAssertion>(
        NodeType::PROGRAM,
        Vector<Ref<DelayAssertion>>{
            CreateRef<AtomicAssertion>(TokenType::INTEGER, u32(23)),
        })
        ->Assert(CreateRef<BlockNode>(blockNode));
}

TEST(BlockNodeCompressTest, HasBlockCode)
{
    BlockNode blockNode(
        NodeType::PROGRAM,
        "3 + (2 + 4)");
    blockNode.Compress();

    CreateRef<BlockAssertion>(
        NodeType::PROGRAM,
        Vector<Ref<DelayAssertion>>{
            CreateRef<AtomicAssertion>(TokenType::INTEGER, u32(3)),
            CreateRef<AtomicAssertion>(TokenType::OPERATOR, "+"),
            CreateRef<BlockAssertion>(
                NodeType::EXPRESSION,
                Vector<Ref<DelayAssertion>>{
                    CreateRef<AtomicAssertion>(TokenType::INTEGER, u32(2)),
                    CreateRef<AtomicAssertion>(TokenType::OPERATOR, "+"),
                    CreateRef<AtomicAssertion>(TokenType::INTEGER, u32(4)),
                }),
        })
        ->Assert(CreateRef<BlockNode>(blockNode));
}