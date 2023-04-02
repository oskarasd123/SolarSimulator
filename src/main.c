#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"

#include "graphics/vertex.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/buffers.h"
#include "graphics/vertex_array.h"

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
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE);
    glDebugMessageCallback(message_callback, NULL);

    const char* vendor_str = (const char*)glGetString(GL_VENDOR);
    const char* version_str = (const char*)glGetString(GL_VERSION);
    const char* renderer_str = (const char*)glGetString(GL_RENDERER);
    printf("Vendor: %s\nVersion: %s\nRenderer: %s\n", vendor_str, version_str, renderer_str);

    struct vertex vertices[] = {
        {.pos = {.x =  0.5f, .y =  0.5f, .z = 0.0f}, .uv = {.x = 1.0f, .y = 1.0f}},
        {.pos = {.x =  0.5f, .y = -0.5f, .z = 0.0f}, .uv = {.x = 1.0f, .y = 0.0f}},
        {.pos = {.x = -0.5f, .y = -0.5f, .z = 0.0f}, .uv = {.x = 0.0f, .y = 0.0f}},
        {.pos = {.x = -0.5f, .y =  0.5f, .z = 0.0f}, .uv = {.x = 0.0f, .y = 1.0f}}
    };

    uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    buffer_handle_t buffers[2];
    size_t sizes[2] = {sizeof(vertices), sizeof(indices)};
    void* data[2] = {vertices, indices};
    buffers_init(2, buffers, sizes, data);

    struct vertex_array vao;
    vertex_array_init(&vao);

    struct vertex_layout layout;
    vertex_layout_init(&layout);
    vertex_layout_add(&layout, 3, GL_FLOAT, false, 0);
    vertex_layout_add(&layout, 2, GL_FLOAT, false, offsetof(struct vertex, uv));
    
    vertex_array_add(&vao, &layout);
    vertex_array_bind(&vao);
    vertex_array_set(&vao, sizeof(struct vertex), buffers[BUFFER_ID_VBO], buffers[BUFFER_ID_IBO]);

    texture_t texture = texture_init("wall.jpg");
    texture_bind(texture, 0);

    struct shader shader;
    shader_init(&shader, "basic.shader");
    shader_bind(&shader);
    shader_set_1i(&shader, "u_Texture", 0);

    struct mat4 transform;
    mat4_identity(&transform);
    struct vec3 translation;
    vec3_init(&translation, 0.5f, -0.25f, 0.0f);
    mat4_translation(&transform, &translation);
    struct vec3 scale;
    vec3_init(&scale, 4.0f, 4.0f, 4.0f);
    mat4_scale(&transform, &scale);
    shader_set_mat4(&shader, "u_Transform", &transform);

    struct mat4 projection = mat4_perspective(70, 960.0f / 540.0f, 0.0f, 1.0f);
    shader_set_mat4(&shader, "u_Projection", &projection);

    struct vec3 camera;
    vec3_init(&camera, 5.0f, 5.0f, 0.0f);
    struct vec3 object;
    vec3_init(&object, 0.0f, 0.0f, 0.0f);
    struct vec3 up;
    vec3_init(&up, 0.0f, 1.0f, 0.0f);
    struct mat4 view = mat4_look_at(camera, object, up);
    
    struct vec3 x_axis;
    vec3_init(&x_axis, 1.0f, 0.0f, 0.0f);
    mat4_rotation(&view, 90.0f, &x_axis);

    shader_set_mat4(&shader, "u_View", &view);

    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        struct vec3 axis;
        vec3_init(&axis, 0.0f, 0.0f, 1.0f);
        mat4_rotation(&transform, (float)glfwGetTime(), &axis);
        shader_set_mat4(&shader, "u_Transform", &transform);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (glfwGetKey(window, GLFW_KEY_Q)) {
            glfwSetWindowShouldClose(window, true);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    texture_free(texture);
    shader_free(&shader);
    vertex_array_free(&vao);
    buffers_free(2, buffers);

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