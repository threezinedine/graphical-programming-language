#include "parser/node.h"

namespace ntt
{
    void Node::AddError(ErrorType error)
    {
        if (error == ErrorType::NO_ERROR)
        {
            return;
        }

        b8 found = NTT_FALSE;
        for (const auto &existingError : m_errors)
        {
            if (existingError == error)
            {
                found = NTT_TRUE;
                break;
            }
        }

        if (!found)
        {
            m_errors.push_back(error);
        }
    }

    JSON Node::ToErrorJSON()
    {
        JSON json;
        json = JSON::array();
        for (const auto &error : m_errors)
        {
            json.push_back(ErrorTypeToString(error));
        }
        return json;
    }
} // namespace ntt
