#include "vec4.h"

#if VEC4_USE_SIMD
#include <immintrin.h>
#endif

void vec4_init(struct vec4* vec, float x, float y, float z, float w)
{
    vec->x = x;
    vec->y = y;
    vec->z = z;
    vec->w = w;
}

void vec4_add(struct vec4* vec, struct vec4* other)
{
#if VEC4_USE_SIMD
    __m128 a = _mm_load_ps((const float*)vec);
    __m128 b = _mm_load_ps((const float*)other);
    __m128 res = _mm_add_ps(a, b);
    _mm_store_ps((float*)vec, res);
#else
    vec->x += other->x;
    vec->y += other->y;
    vec->z += other->z;
    vec->w += other->w;
#endif
}

void vec4_sub(struct vec4* vec, struct vec4* other)
{
#if VEC4_USE_SIMD
    __m128 a = _mm_load_ps((const float*)vec);
    __m128 b = _mm_load_ps((const float*)other);
    __m128 res = _mm_sub_ps(a, b);
    _mm_store_ps((float*)vec, res);
#else
    vec->x -= other->x;
    vec->y -= other->y;
    vec->z -= other->z;
    vec->w -= other->w;
#endif
}

void vec4_mul(struct vec4* vec, struct vec4* other)
{
#if VEC4_USE_SIMD
    __m128 a = _mm_load_ps((const float*)vec);
    __m128 b = _mm_load_ps((const float*)other);
    __m128 res = _mm_mul_ps(a, b);
    _mm_store_ps((float*)vec, res);
#else
    vec->x *= other->x;
    vec->y *= other->y;
    vec->z *= other->z;
    vec->w *= other->w;
#endif
}

void vec4_div(struct vec4* vec, struct vec4* other)
{
#if VEC4_USE_SIMD
    __m128 a = _mm_load_ps((const float*)vec);
    __m128 b = _mm_load_ps((const float*)other);
    __m128 res = _mm_div_ps(a, b);
    _mm_store_ps((float*)vec, res);
#else
    vec->x /= other->x;
    vec->y /= other->y;
    vec->z /= other->z;
    vec->w /= other->w;
#endif
}