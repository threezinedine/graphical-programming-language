#pragma once
#include "pch.h"
#include "parser/node.h"
#include "node_pattern.h"

namespace ntt
{
    /**
     * Used for matching the node content with gex patterns. This method tries to
     *      mimic the way the regex library works, but use simpler interface.
     */
    class NodeGex
    {
    public:
        NodeGex();
        ~NodeGex();

        /**
         * The main interface for matching the node content with gex patterns.
         *
         * @param inNodes All inputs which will be used for matching.
         * @param outMatches The result of all matching positions.
         *
         * @return Whether any match is found.
         *
         */
        b8 Match(const Vector<Node> &inNodes, Vector<Node> &outMatches, const Vector<NodePattern> &patterns);

    private:
    };
} // namespace ntt
