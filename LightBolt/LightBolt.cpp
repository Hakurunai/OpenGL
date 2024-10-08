#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

int main()
{
    GLFWwindow* window;

    try
    {
        if (!glfwInit())
            throw std::runtime_error("Failed to init glfw");
    }
    catch (std::exception e)
    {
        std::cout << e.what();
    }

    window = glfwCreateWindow(640, 480, "LightBolt", NULL, NULL);

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}