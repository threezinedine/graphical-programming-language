#pragma once
#include "pch.h"
#include "parser/node.h"

namespace ntt
{
    /**
     * The interface for defining the way each node or list of nodes should be matched against a pattern.
     */
    class NodePattern
    {
    public:
        virtual ~NodePattern() = default;

        /**
         * Checks if the nodes starting from the given index match the pattern.
         *
         * @param inNodes The vector of nodes to match against (this is original nodes).
         *      The nodes are not copied, so the pattern should not modify them.
         * @param startIndex The index in the inNodes from which the matching should start.
         *      With the GetPatternLength() method, the pattern can determine how many nodes
         *      it expects to match and the final index it should check. This value will be
         *      modifed to the next index after the matched nodes if the match is successful.
         * @return Whether the nodes match the pattern starting from the given index.
         */
        virtual b8 Match(const Vector<Ref<Node>> &inNodes, u32 &startIndex) = 0;
    };

    class NodePatternBuilder
    {
    public:
        virtual Ref<NodePattern> Build() = 0;
    };
} // namespace ntt
