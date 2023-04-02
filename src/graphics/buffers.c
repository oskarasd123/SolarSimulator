#include "buffers.h"

#include <GL/glew.h>

void buffers_init(uint32_t n, buffer_handle_t* buffers, size_t* sizes, void** data)
{
    glCreateBuffers(n, buffers);
    for (uint32_t i = 0; i < n; ++i) {
        glNamedBufferData(buffers[i], sizes[i], data[i], GL_STATIC_DRAW);
    }
}

void buffers_free(uint32_t n, buffer_handle_t* buffers)
{
    glDeleteBuffers(n, buffers);
}