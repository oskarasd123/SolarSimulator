#ifndef TEXTURE_H
#define TEXTURE_H

#include <inttypes.h>

typedef uint32_t texture_t;

texture_t texture_init(const char* texture_path);
void texture_delete(texture_t texture);
void texture_bind(texture_t texture, uint32_t slot);

#endif