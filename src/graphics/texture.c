#include "texture.h"

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/stb_image.h"

texture_handle_t texture_init(const char* texture_path)
{
    uint32_t texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    uint8_t* data = stbi_load(texture_path, &width, &height, &channels, 0);
    if (data) {
        glTextureStorage2D(texture, 1, GL_RGB8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(texture);
    } else {
        fputs("Failed to load the texture!\n", stderr);
        abort();
    }

    stbi_image_free(data);
    return texture;
}

void texture_delete(texture_handle_t texture)
{
    glDeleteTextures(1, &texture);
}

void texture_bind(texture_handle_t texture, uint32_t slot)
{
    glBindTextureUnit(slot, texture);
}