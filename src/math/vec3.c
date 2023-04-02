#include "vec3.h"

#if VEC3_USE_SIMD
#include <immintrin.h>
#endif

#include <math.h>

void vec3_init(struct vec3* vec, float x, float y, float z)
{
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

void vec3_add(struct vec3* vec, struct vec3* other)
{
#if VEC3_USE_SIMD
    __m128 a = _mm_load_ps((const float*)vec);
    __m128 b = _mm_load_ps((const float*)other);
    __m128 res = _mm_add_ps(a, b);
    _mm_store_ps((float*)vec, res);
#else
    vec->x += other->x;
    vec->y += other->y;
    vec->z += other->z;
#endif
}

void vec3_sub(struct vec3* vec, struct vec3* other)
{
#if VEC3_USE_SIMD
    __m128 a = _mm_load_ps((const float*)vec);
    __m128 b = _mm_load_ps((const float*)other);
    __m128 res = _mm_sub_ps(a, b);
    _mm_store_ps((float*)vec, res);
#else
    vec->x -= other->x;
    vec->y -= other->y;
    vec->z -= other->z;
#endif
}

void vec3_mul(struct vec3* vec, struct vec3* other)
{
#if VEC3_USE_SIMD
    __m128 a = _mm_load_ps((const float*)vec);
    __m128 b = _mm_load_ps((const float*)other);
    __m128 res = _mm_mul_ps(a, b);
    _mm_store_ps((float*)vec, res);
#else
    vec->x *= other->x;
    vec->y *= other->y;
    vec->z *= other->z;
#endif
}

void vec3_div(struct vec3* vec, struct vec3* other)
{
#if VEC3_USE_SIMD
    __m128 a = _mm_load_ps((const float*)vec);
    __m128 b = _mm_load_ps((const float*)other);
    __m128 res = _mm_div_ps(a, b);
    _mm_store_ps((float*)vec, res);
#else
    vec->x /= other->x;
    vec->y /= other->y;
    vec->z /= other->z;
#endif
}

float vec3_magnitude(const struct vec3* vec)
{
    return sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

struct vec3 vec3_normalize(const struct vec3* vec)
{
    const float length = vec3_magnitude(vec);
    struct vec3 result;
    vec3_init(&result, vec->x / length, vec->y / length, vec->z / length);
    return result;
}

void vec3_normalized(struct vec3* vec)
{
    const float length = vec3_magnitude(vec);
    vec->x /= length;
    vec->y /= length;
    vec->z /= length;
}

struct vec3 vec3_cross(const struct vec3* left, const struct vec3* right)
{
    struct vec3 result;
    vec3_init(&result, left->y * right->z - left->z * right->z, left->z * right->x - left->x * right->z, left->x * right->y - left->y * right->x);
    return result;
}