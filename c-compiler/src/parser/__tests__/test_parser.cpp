#include "test_common.h"
#include "parser/blockNode.h"
#include "assertions.h"

using namespace ntt;

TEST(BlockNodeTest, CreateWithContent)
{
    BlockNode blockNode(NodeType::PROGRAM, "23");
    blockNode.Compress();

    CreateRef<ProgramAssertion>(
        Vector<Ref<DelayAssertion>>{
            CreateRef<AtomicAssertion>(TokenType::INTEGER, u32(23)),
        })
        ->Assert(CreateRef<BlockNode>(blockNode));
}