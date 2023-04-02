#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"

#include "graphics/vertex.h"
#include "graphics/shader.h"

static void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);

enum buffer_id
{
    BUFFER_ID_VBO = 0,
    BUFFER_ID_IBO = 1
};

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

    uint32_t buffers[2];
    glCreateBuffers(2, buffers);
    glNamedBufferData(buffers[BUFFER_ID_VBO], sizeof(vertices), vertices, GL_STATIC_DRAW);
    glNamedBufferData(buffers[BUFFER_ID_IBO], sizeof(indices), indices, GL_STATIC_DRAW);

    uint32_t VAO;
    glCreateVertexArrays(1, &VAO);

    glVertexArrayVertexBuffer(VAO, 0, buffers[BUFFER_ID_VBO], 0, sizeof(struct vertex));
    glVertexArrayElementBuffer(VAO, buffers[BUFFER_ID_IBO]);

    glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, offsetof(struct vertex, uv));

    glEnableVertexArrayAttrib(VAO, 0);
    glEnableVertexArrayAttrib(VAO, 1);

    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribBinding(VAO, 1, 0);

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
        abort();
    }

    stbi_image_free(data);

    struct shader shader;
    shader_init(&shader, "basic.shader");
    shader_bind(&shader);
    shader_set_1i(&shader, "u_Texture", 0);

    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
    glBindVertexArray(VAO);
    
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

    glDeleteTextures(1, &texture);
    shader_delete(&shader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, buffers);

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