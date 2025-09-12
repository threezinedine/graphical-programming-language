#include "parser/error.h"

namespace ntt
{
    String ErrorTypeToString(ErrorType error)
    {
        switch (error)
        {
        case ErrorType::MISSING_END_BRACKET:
            return "Missing closing bracket";
        case ErrorType::MISSING_LEFT_OPERAND:
            return "Missing left operand";
        case ErrorType::MISSING_RIGHT_OPERAND:
            return "Missing right operand";
        case ErrorType::MISSING_SEMICOLON:
            return "Missing semicolon";
        case ErrorType::MISSING_BLOCK:
            return "Missing block";

        case ErrorType::MISSING_VARIABLE_NAME:
            return "Missing variable name";

        case ErrorType::MISSING_CONDITION:
            return "Missing condition";

        case ErrorType::REDUNDANT_DELIMITER:
            return "Redundant delimiter";

        case ErrorType::NO_ERROR:
            return "No error";
        default:
            return "Unknown error";
        }
    }
}