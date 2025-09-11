#include "parser/function_call.h"

namespace ntt
{
    FunctionCallNode::FunctionCallNode(const Ref<Node> &function, const Vector<Ref<Node>> &arguments)
        : m_function(function), m_arguments(arguments)
    {
    }

    FunctionCallNode::~FunctionCallNode()
    {
    }

    void FunctionCallNode::Compress()
    {
    }

    void FunctionCallNode::Parse()
    {
    }

    JSON FunctionCallNode::ToJSON()
    {
        JSON json;
        json["type"] = "FunctionCall";
        json["function"] = m_function->ToJSON();
        json["arguments"] = JSON::array();
        json["errors"] = ToErrorJSON();
        for (const Ref<Node> &arg : m_arguments)
        {
            json["arguments"].push_back(arg->ToJSON());
        }
        return json;
    }
}