#include "chip8.h"

#define ever ;;
#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 32

void frameBufferSizeCallback(GLFWwindow*, int width, int height);
void processInput(GLFWwindow*);

int main()
{
    Chip8 chip8;

    if(chip8.LoadGame() != 0)
    {
        std::fprintf(stderr, "Failed to load the ROM\n");
        return -4;
    }

    chip8.Display();

    return 0;
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

