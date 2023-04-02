#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <inttypes.h>
#include <stdbool.h>

#include "buffers.h"

struct vertex_element
{
    uint32_t attribute_index;
    int32_t components;
    uint32_t type;
    bool normalized;
    uint32_t offset;
};

struct vertex_layout
{
    uint32_t index;

    uint32_t size;
    uint32_t capacity;
    struct vertex_element* data;
};

struct vertex_array
{
    uint32_t handle;
    
    struct vertex_layout* layout;
};

void vertex_layout_init(struct vertex_layout* layout);
void vertex_layout_free(struct vertex_layout* layout);
void vertex_layout_add(struct vertex_layout* layout, int32_t components, uint32_t type, bool normalized, uint32_t offset);

void vertex_array_init(struct vertex_array* vao);
void vertex_array_free(struct vertex_array* vao);
void vertex_array_bind(struct vertex_array* vao);
void vertex_array_add(struct vertex_array* vao, struct vertex_layout* layout);
void vertex_array_set(struct vertex_array* vao, uint32_t vertex_size, buffer_handle_t vbo, buffer_handle_t ibo);

#endif