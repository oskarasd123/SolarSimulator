#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}