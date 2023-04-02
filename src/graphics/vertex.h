#ifndef VERTEX_H
#define VERTEX_H

#include "../math/vec2.h"
#include "../math/vec3.h"

struct vertex
{
    struct vec3 pos;
    struct vec2 uv;
};

#endif