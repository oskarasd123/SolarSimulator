#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

#include "vertex.h"

static void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
static int starts_with(const char* a, const char* b);

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    if (glfwInit() != GLFW_TRUE) {
        fputs("Failed to initialize GLFW!", stderr);
        return EXIT_FAILURE;
    }

    const uint32_t WIDTH = 960;
    const uint32_t HEIGHT = 540;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Solar System Simulator", NULL, NULL);
    if (!window) {
        fputs("Failed to create GLFW window!", stderr);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        fputs("Failed to initialize GLEW!", stderr);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
    glDebugMessageCallback(message_callback, NULL);

    const char* vendor_str = (const char*)glGetString(GL_VENDOR);
    const char* version_str = (const char*)glGetString(GL_VERSION);
    const char* renderer_str = (const char*)glGetString(GL_RENDERER);
    printf("Vendor: %s\nVersion: %s\nRenderer: %s\n", vendor_str, version_str, renderer_str);

    struct vertex vertices[] = {
        {.pos = {.x =  0.5f, .y =  0.5f}, .uv = {.x = 1.0f, .y = 1.0f}},
        {.pos = {.x =  0.5f, .y = -0.5f}, .uv = {.x = 1.0f, .y = 0.0f}},
        {.pos = {.x = -0.5f, .y = -0.5f}, .uv = {.x = 0.0f, .y = 0.0f}},
        {.pos = {.x = -0.5f, .y =  0.5f}, .uv = {.x = 0.0f, .y = 1.0f}}
    };

    uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    uint32_t VBO;
    glCreateBuffers(1, &VBO);
    glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

    uint32_t IBO;
    glCreateBuffers(1, &IBO);
    glNamedBufferData(IBO, sizeof(indices), indices, GL_STATIC_DRAW);

    uint32_t VAO;
    glCreateVertexArrays(1, &VAO);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(struct vertex));
    glVertexArrayElementBuffer(VAO, IBO);

    glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, offsetof(struct vertex, uv));

    glEnableVertexArrayAttrib(VAO, 0);
    glEnableVertexArrayAttrib(VAO, 1);

    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribBinding(VAO, 1, 0);

    FILE* file = fopen("basic.shader", "r");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* shaders[2];
    shaders[0] = calloc(size, sizeof(char));
    shaders[1] = calloc(size, sizeof(char));
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
                goto cleanup2;
            }
            strcat(shaders[shader_write_index], line);
        }
    }

    fclose(file);

    uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, (const char**) &shaders[0], NULL);
    glCompileShader(vs);
    int success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        int32_t length;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length);
        char* error_message = calloc(length, sizeof(char));
        glGetShaderInfoLog(vs, length, &length, error_message);
        fprintf(stderr, "Failed to compile vertex shader:\n%s\n", error_message);
        free(error_message);
        glDeleteShader(vs);
        goto cleanup2;
    }

    uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, (const char**) &shaders[1], NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        int32_t length;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &length);
        char* error_message = calloc(length, sizeof(char));
        glGetShaderInfoLog(fs, length, &length, error_message);
        fprintf(stderr, "Failed to compile fragment shader:\n%s\n", error_message);
        free(error_message);
        glDeleteShader(fs);
        goto cleanup2;
    }

    uint32_t shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        int32_t length;
        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &length);
        char* error_message = calloc(length, sizeof(char));
        fprintf(stderr, "Failed to link shader program:\n%s\n", error_message);
        free(error_message);
        glDeleteProgram(shader_program);
        goto cleanup1;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    free(shaders[0]);
    free(shaders[1]);

    uint32_t texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    uint8_t* data = stbi_load("wall.jpg", &width, &height, &channels, 0);
    if (data) {
        glTextureStorage2D(texture, 1, GL_RGB8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(texture);
        glBindTextureUnit(0, texture);
    } else {
        fputs("Failed to load the texture!\n", stderr);
        goto cleanup;
    }

    stbi_image_free(data);

    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
    glUseProgram(shader_program);
    glBindVertexArray(VAO);

    int32_t location = glGetUniformLocation(shader_program, "u_Texture");
    if (location < 0) {
        fprintf(stderr, "Failed to find uniform location '%s'!\n", "u_Texture");
        goto cleanup;
    }
    glUniform1i(location, 0);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (glfwGetKey(window, GLFW_KEY_Q)) {
            glfwSetWindowShouldClose(window, true);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

cleanup:
    glDeleteTextures(1, &texture);
cleanup1:
    glDeleteProgram(shader_program);
cleanup2:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

static void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
    (void) id;
    (void) length;
    (void) user_param;

	const char* src_str;
    switch (source)
	{
    case GL_DEBUG_SOURCE_API: src_str = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: src_str = "WINDOW SYSTEM"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: src_str = "SHADER COMPILER"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: src_str = "THIRD PARTY"; break;
    case GL_DEBUG_SOURCE_APPLICATION: src_str = "APPLICATION"; break;
    case GL_DEBUG_SOURCE_OTHER: src_str = "OTHER"; break;
    default: src_str = "UNKNOWN"; break;
	}

	const char* type_str;
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: type_str = "ERROR"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "DEPRECATED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_str = "UNDEFINED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_PORTABILITY: type_str = "PORTABILITY"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: type_str = "PERFORMANCE"; break;
    case GL_DEBUG_TYPE_MARKER: type_str = "MARKER"; break;
    case GL_DEBUG_TYPE_OTHER: type_str = "OTHER"; break;
    default: type_str = "UNKNOWN"; break;
    }

	const char* severity_str;
    switch (severity) {
    case GL_DEBUG_SEVERITY_NOTIFICATION: severity_str = "NOTIFICATION"; break;
    case GL_DEBUG_SEVERITY_LOW: severity_str = "LOW"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: severity_str = "MEDIUM"; break;
    case GL_DEBUG_SEVERITY_HIGH: severity_str = "HIGH"; break;
    default: severity_str = "UNKNOWN"; break;
    }

    printf("[%s][%s][%s]: %s\n", src_str, type_str, severity_str, message);
}

static int starts_with(const char* a, const char* b)
{
    return strncmp(a, b, strlen(b));
}