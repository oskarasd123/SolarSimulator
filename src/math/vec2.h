#ifndef VEC2_H
#define VEC2_H

#include "../config.h"

struct vec2
{
    float x;
    float y;

#if VEC2_USE_SIMD
    /* Padding to align it to 128 bits */
    float _[2];
#endif
};

void vec2_init(struct vec2* vec, float x, float y);
void vec2_add(struct vec2* vec, struct vec2* other);
void vec2_sub(struct vec2* vec, struct vec2* other);
void vec2_mul(struct vec2* vec, struct vec2* other);
void vec2_div(struct vec2* vec, struct vec2* other);

#endif