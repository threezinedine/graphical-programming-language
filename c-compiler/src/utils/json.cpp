#include "utils/json.h"

namespace ntt
{
    void print(const JSON &json)
    {
        printf("%s\n", json.dump(4).c_str());
    }
} // namespace ntt
