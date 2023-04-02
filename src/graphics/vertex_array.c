#include "vertex_array.h"

#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

static void vertex_layout_reallocate(struct vertex_layout* layout, uint32_t new_size)
{
    puts("Reallocation time!");
    layout->data = realloc(layout->data, new_size);
    layout->capacity = new_size;
}

void vertex_layout_init(struct vertex_layout* layout)
{
    layout->index = 0;
    layout->size = 0;
    layout->capacity = 4;
    layout->data = calloc(layout->capacity, sizeof(struct vertex_element));
}

void vertex_layout_free(struct vertex_layout* layout)
{
    layout->index = 0;
    layout->size = 0;
    layout->capacity = 0;
    free(layout->data);
}

void vertex_layout_add(struct vertex_layout* layout, int32_t components, uint32_t type, bool normalized, uint32_t offset)
{
    struct vertex_element element;
    element.attribute_index = layout->index++;
    element.components = components;
    element.type = type;
    element.normalized = normalized;
    element.offset = offset;

    if (layout->size >= layout->capacity) {
        vertex_layout_reallocate(layout, 2 * layout->capacity);
    }

    layout->data[layout->size++] = element;
}

void vertex_array_init(struct vertex_array* vao)
{
    glCreateVertexArrays(1, &vao->handle);
    vao->layout = NULL;
}

void vertex_array_free(struct vertex_array* vao)
{
    glDeleteVertexArrays(1, &vao->handle);
    vertex_layout_free(vao->layout);
}

void vertex_array_bind(struct vertex_array* vao)
{
    glBindVertexArray(vao->handle);
}

void vertex_array_add(struct vertex_array* vao, struct vertex_layout* layout)
{
    vao->layout = layout;
    for (uint32_t i = 0; i < layout->size; i++) {
        const struct vertex_element element = layout->data[i];
        glVertexArrayAttribFormat(vao->handle, element.attribute_index, element.components, element.type, element.normalized ? GL_TRUE : GL_FALSE, element.offset);
        glEnableVertexArrayAttrib(vao->handle, element.attribute_index);
        glVertexArrayAttribBinding(vao->handle, element.attribute_index, 0);
    }
}

void vertex_array_set(struct vertex_array* vao, uint32_t vertex_size, buffer_handle_t vbo, buffer_handle_t ibo)
{
    glVertexArrayVertexBuffer(vao->handle, 0, vbo, 0, vertex_size);
    glVertexArrayElementBuffer(vao->handle, ibo);
}