#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "chip8.h"

#define ever ;;
#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 32

void frameBufferSizeCallback(GLFWwindow*, int width, int height);

int main()
{
    int displayScale  = 14;
    int displayWidth  = WINDOW_WIDTH  * displayScale;
    int displayHeight = WINDOW_HEIGHT * displayScale;

    Chip8 chip8;

    if(chip8.LoadGame() != 0)
    {
        std::fprintf(stderr, "Failed to load the ROM\n");
        return -4;
    }

    if(!glfwInit())
    {
        std::fprintf(stderr, "Failed to initialize GLFW\n");
        return -2;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(displayWidth, displayHeight, "Chip8 Emulator by Jacob Laws", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if(window == nullptr)
    {
        std::fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -3;
    }

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::fprintf(stderr, "Failed to initialize GLAD\n");
        return -2;
    }

    glViewport(0, 0, displayWidth, displayHeight);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

