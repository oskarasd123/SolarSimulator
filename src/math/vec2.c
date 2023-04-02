#include "vec2.h"

#if VEC2_USE_SIMD
#include <immintrin.h>
#endif

void vec2_init(struct vec2* vec, float x, float y)
{
    vec->x = x;
    vec->y = y;
}

void vec2_add(struct vec2* vec, struct vec2* other)
{
#if VEC2_USE_SIMD
    __m128 a = _mm_loadu_ps((const float*)vec);
    __m128 b = _mm_loadu_ps((const float*)other);
    __m128 res = _mm_add_ps(a, b);
    _mm_storeu_ps((float*)vec, res);
#else
    vec->x += other->x;
    vec->y += other->y;
#endif
}

void vec2_sub(struct vec2* vec, struct vec2* other)
{
#if VEC2_USE_SIMD
    __m128 a = _mm_loadu_ps((const float*)vec);
    __m128 b = _mm_loadu_ps((const float*)other);
    __m128 res = _mm_sub_ps(a, b);
    _mm_storeu_ps((float*)vec, res);
#else
    vec->x -= other->x;
    vec->y -= other->y;
#endif
}

void vec2_mul(struct vec2* vec, struct vec2* other)
{
#if VEC2_USE_SIMD
    __m128 a = _mm_loadu_ps((const float*)vec);
    __m128 b = _mm_loadu_ps((const float*)other);
    __m128 res = _mm_mul_ps(a, b);
    _mm_storeu_ps((float*)vec, res);
#else
    vec->x *= other->x;
    vec->y *= other->y;
#endif
}

void vec2_div(struct vec2* vec, struct vec2* other)
{
#if VEC2_USE_SIMD
    __m128 a = _mm_loadu_ps((const float*)vec);
    __m128 b = _mm_loadu_ps((const float*)other);
    __m128 res = _mm_div_ps(a, b);
    _mm_storeu_ps((float*)vec, res);
#else
    vec->x /= other->x;
    vec->y /= other->y;
#endif
}