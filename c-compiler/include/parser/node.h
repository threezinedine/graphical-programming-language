#pragma once
#include "pch.h"
#include "nodeType.h"
#include "error.h"

namespace ntt
{
    /**
     * Simplest interface for all AST nodes.
     */
    class Node
    {
    public:
        virtual ~Node() = default;

        /**
         * Get the type of this node.
         * @return The type of this node.
         */
        virtual NodeType GetType() const = 0;

        /**
         * Convert this node to a JSON representation.
         * @return The JSON representation of this node.
         */
        virtual JSON ToJSON() = 0;

        /**
         * Used for grouping nodes which are inside the brackets together into
         *      another BlockType node.
         */
        virtual void Compress() = 0;

        /**
         * Actually perform the parsing of this node.
         */
        virtual void Parse() = 0;

        const Vector<ErrorType> &GetErrors() const { return m_errors; }
        void AddError(ErrorType error);
        void ClearErrors() { m_errors.clear(); }
        b8 HasErrors() const { return !m_errors.empty(); }

        JSON ToErrorJSON();

    private:
        Vector<ErrorType> m_errors;
    };
} // namespace ntt
