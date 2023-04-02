#ifndef VEC4_H
#define VEC4_H

#include "../config.h"

struct vec4
{
    float x;
    float y;
    float z;
    float w;
};

void vec4_init(struct vec4* vec, float x, float y, float z, float w);
void vec4_add(struct vec4* vec, struct vec4* other);
void vec4_sub(struct vec4* vec, struct vec4* other);
void vec4_mul(struct vec4* vec, struct vec4* other);
void vec4_div(struct vec4* vec, struct vec4* other);

#endif