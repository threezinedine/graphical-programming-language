#include <cstdio>
#include <vector>
#include <string>

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

#define NTT_TRUE true
#define NTT_FALSE false

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