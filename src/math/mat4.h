#ifndef MAT4_H
#define MAT4_H

#include "vec3.h"
#include "vec4.h"

struct mat4
{
    union
    {
        float elements[16];
        struct vec4 columns[4];
    };
};

void mat4_init(struct mat4* matrix, float diagonal);
void mat4_identity(struct mat4* matrix);
void mat4_mul(struct mat4* left, const struct mat4* right);

struct mat4 mat4_orthographic(float left, float right, float bottom, float top, float near, float far);
struct mat4 mat4_perspective(float fov, float aspect_ratio, float near, float far);

struct mat4 mat4_look_at(struct vec3 camera, struct vec3 object, struct vec3 up);

void mat4_translation(struct mat4* matrix, const struct vec3* translation);
void mat4_rotation(struct mat4* matrix, float angle, const struct vec3* axis);
void mat4_scale(struct mat4* matrix, const struct vec3* scale);

#endif