#pragma once
#include <cstdio>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <memory>

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef long long i64;
typedef int i32;
typedef short i16;
typedef char i8;

typedef float f32;
typedef double f64;

typedef bool b8;

typedef std::string String;
typedef nlohmann::json JSON;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
Scope<T> CreateScope(Args &&...args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
Ref<T> CreateRef(Args &&...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#define NTT_TRUE b8(true)
#define NTT_FALSE b8(false)

#define NTT_NULL nullptr

#define NTT_ASSERT(exp) \
    if (!(exp))         \
    {                   \
        __debugbreak(); \
    }

#define NTT_ASSERT_MSG(exp, msg) \
    if (!(exp))                  \
    {                            \
        printf("%s\n", msg);     \
        __debugbreak();          \
    }

#define STRINGIFY(x) #x