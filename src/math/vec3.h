#ifndef VEC3_H
#define VEC3_H

#include "../config.h"

struct vec3
{
    float x;
    float y;
    float z;

    /* Padding to align to 128 bits */
#if VEC3_USE_SIMD
    float _;
#endif
};

void vec3_init(struct vec3* vec, float x, float y, float z);
void vec3_add(struct vec3* vec, struct vec3* other);
void vec3_sub(struct vec3* vec, struct vec3* other);
void vec3_mul(struct vec3* vec, struct vec3* other);
void vec3_div(struct vec3* vec, struct vec3* other);

#endif