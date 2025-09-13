#pragma once

#include "test_common.h"
#include "parser/nodegex/single_node_pattern.h"
#include "tokenizer/tokenType.h"
#include "parser/atomic.h"

using namespace ntt;

#define CREATE_ATOMIC_NODE(name, type, value) \
    Token token##name(type, 0);               \
    token##name.SetValue(value);              \
    Ref<Node> name = CreateRef<Atomic>(NodeType::ATOMIC, token##name);
