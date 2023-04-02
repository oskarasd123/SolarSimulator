#include "vec3.h"

#if VEC3_USE_SIMD
#include <immintrin.h>
#endif

void vec3_init(struct vec3* vec, float x, float y, float z)
{
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

void vec3_add(struct vec3* vec, struct vec3* other)
{
#if VEC3_USE_SIMD
    __m128 a = _mm_load_ps(vec);
    __m128 b = _mm_load_ps(other);
    __m128 res = _mm_add_ps(a, b);
    _mm_store_ps(vec, res);
#else
    vec->x += other->x;
    vec->y += other->y;
    vec->z += other->z;
#endif
}

void vec3_sub(struct vec3* vec, struct vec3* other)
{
#if VEC3_USE_SIMD
    __m128 a = _mm_load_ps(vec);
    __m128 b = _mm_load_ps(other);
    __m128 res = _mm_sub_ps(a, b);
    _mm_store_ps(vec, res);
#else
    vec->x -= other->x;
    vec->y -= other->y;
    vec->z -= other->z;
#endif
}

void vec3_mul(struct vec3* vec, struct vec3* other)
{
#if VEC3_USE_SIMD
    __m128 a = _mm_load_ps(vec);
    __m128 b = _mm_load_ps(other);
    __m128 res = _mm_mul_ps(a, b);
    _mm_store_ps(vec, res);
#else
    vec->x *= other->x;
    vec->y *= other->y;
    vec->z *= other->z;
#endif
}

void vec3_div(struct vec3* vec, struct vec3* other)
{
#if VEC3_USE_SIMD
    __m128 a = _mm_load_ps(vec);
    __m128 b = _mm_load_ps(other);
    __m128 res = _mm_div_ps(a, b);
    _mm_store_ps(vec, res);
#else
    vec->x /= other->x;
    vec->y /= other->y;
    vec->z /= other->z;
#endif
}