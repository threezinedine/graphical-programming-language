#include "parser/error.h"

namespace ntt
{
    String ErrorTypeToString(ErrorType error)
    {
        switch (error)
        {
        case ErrorType::MISSING_END_BRACKET:
            return "Missing closing bracket";
        default:
            return "Unknown error";
        }
    }
}