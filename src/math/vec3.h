#ifndef VEC3_H
#define VEC3_H

#include "../config.h"

struct vec3
{
    float x;
    float y;
    float z;
};

void vec3_init(struct vec3* vec, float x, float y, float z);
void vec3_add(struct vec3* vec, struct vec3* other);
void vec3_sub(struct vec3* vec, struct vec3* other);
void vec3_mul(struct vec3* vec, struct vec3* other);
void vec3_div(struct vec3* vec, struct vec3* other);

float vec3_magnitude(const struct vec3* vec);
struct vec3 vec3_normalize(const struct vec3* vec);
void vec3_normalized(struct vec3* vec);
struct vec3 vec3_cross(const struct vec3* left, const struct vec3* right);

#endif