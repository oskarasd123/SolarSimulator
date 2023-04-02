#ifndef SHADER_H
#define SHADER_H

#include <inttypes.h>

#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/vec4.h"

struct shader
{
    uint32_t handle;

    /* TODO HashTable for uniform locations */
};

void shader_init(struct shader* shader, const char* shader_file_path);
void shader_bind(struct shader* shader);
void shader_free(struct shader* shader);

void shader_set_1i(struct shader* shader, const char* name, int value);
void shader_set_2f(struct shader* shader, const char* name, const struct vec2* value);
void shader_set_3f(struct shader* shader, const char* name, const struct vec3* value);
void shader_set_4f(struct shader* shader, const char* name, const struct vec4* value);

#endif