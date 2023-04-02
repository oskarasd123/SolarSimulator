#ifndef BUFFERS_H
#define BUFFERS_H

#include <stdio.h>
#include <inttypes.h>

typedef uint32_t buffer_handle_t;

void buffers_init(uint32_t n, buffer_handle_t* buffers, size_t* sizes, void** data);
void buffers_free(uint32_t n, buffer_handle_t* buffers);

#endif