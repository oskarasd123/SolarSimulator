#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <GL/glew.h>

static void shader_parse(const char* filepath, char** shaders);
static uint32_t compile_shader(const char* source, int shader_type);
static int get_uniform_location(struct shader* shader, const char* name);

void shader_init(struct shader* shader, const char* shader_file_path)
{
    char* shaders[2];
    shader_parse(shader_file_path, shaders);

    uint32_t vs = compile_shader(shaders[0], GL_VERTEX_SHADER);
    uint32_t fs = compile_shader(shaders[1], GL_FRAGMENT_SHADER);

    shader->handle = glCreateProgram();
    glAttachShader(shader->handle, vs);
    glAttachShader(shader->handle, fs);
    glLinkProgram(shader->handle);
    int success;
    glGetProgramiv(shader->handle, GL_LINK_STATUS, &success);
    if (!success) {
        int32_t length;
        glGetProgramiv(shader->handle, GL_INFO_LOG_LENGTH, &length);
        char* error_message = calloc(length, sizeof(char));
        fprintf(stderr, "Failed to link shader program:\n%s\n", error_message);
        free(error_message);
        glDeleteProgram(shader->handle);
        abort();
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    free(shaders[0]);
    free(shaders[1]);
}

void shader_bind(struct shader* shader)
{
    glUseProgram(shader->handle);
}

void shader_free(struct shader* shader)
{
    glDeleteProgram(shader->handle);
}

void shader_set_1i(struct shader* shader, const char* name, int value)
{
    int location = get_uniform_location(shader, name);
    glUniform1i(location, value);
}

void shader_set_2f(struct shader* shader, const char* name, const struct vec2* value)
{
    int location = get_uniform_location(shader, name);
    glUniform2f(location, value->x, value->y);
}

void shader_set_3f(struct shader* shader, const char* name, const struct vec3* value)
{
    int location = get_uniform_location(shader, name);
    glUniform3f(location, value->x, value->y, value->z);
}

void shader_set_4f(struct shader* shader, const char* name, const struct vec4* value)
{
    int location = get_uniform_location(shader, name);
    glUniform4f(location, value->x, value->y, value->z, value->w);
}

void shader_set_mat4(struct shader* shader, const char* name, const struct mat4* matrix)
{
    int location = get_uniform_location(shader, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix->elements);
}

static int get_uniform_location(struct shader* shader, const char* name)
{
    /* TODO: HashTable */
    return glGetUniformLocation(shader->handle, name);
}

static uint32_t compile_shader(const char* source, int shader_type)
{
    uint32_t shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char**) &source, NULL);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int32_t length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* error_message = calloc(length, sizeof(char));
        glGetShaderInfoLog(shader, length, &length, error_message);
        const char* shader_name = (shader_type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
        fprintf(stderr, "Failed to compile %s shader:\n%s\n", shader_name, error_message);
        free(error_message);
        glDeleteShader(shader);
        abort();
    }
    return shader;
}

static int starts_with(const char* a, const char* b)
{
    return strncmp(a, b, strlen(b));
}

static void shader_parse(const char* filepath, char** shaders)
{
    FILE* file = fopen(filepath, "r");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    shaders[0] = calloc(size, sizeof(char));
    shaders[1] = calloc(size, sizeof(char));
    if (!shaders[0] || !shaders[1]) {
        abort();
        return;
    }
    int32_t shader_write_index = -1;

    char line[256];
    while (fgets(line, sizeof(line), file)) 
    {
        if (starts_with(line, "#shader vertex") == 0) {
            shader_write_index = 0;
        } else if (starts_with(line, "#shader fragment") == 0) {
            shader_write_index = 1;
        } else {
            if (shader_write_index < 0) {
                fputs("No shader directive found in the shader file!\n", stderr);
                abort();
            }
            strcat(shaders[shader_write_index], line);
        }
    }

    fclose(file);
}