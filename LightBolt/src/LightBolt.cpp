#include <glad/glad.h> //glad need to be include before glfw
#include <GLFW/glfw3.h>

//std
#include <stdexcept>
#include <iostream>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

void Framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
    try
    {
        if (!glfwInit())
            throw std::runtime_error("Failed to init glfw");
    }
    catch (std::exception e)
    {
        std::cout << e.what();
    }
    //OpenGL 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //GLFW 3.4
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 4);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __Apple__
    #if TARGET_OS_MAC
    //if compiled on mac OS x
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif __TARGET_OS_MAC
#endif __APPLE__

    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "LightBolt", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    //init Glad to load OpenGL functions for us
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    //if the window is resized, we need to resize the viewport too
    glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);


        glClear(GL_COLOR_BUFFER_BIT);


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}


void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void DrawTriangle()
{
    //x,y,z format
    float vertices[] = 
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    //preparing to sending data to the GPU
    unsigned int vertexBufferObect; //VBO
    glGenBuffers(1, &vertexBufferObect);

    //Actions on GL_ARRAY_BUFFER can configure vertexBufferObject
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObect);

    //Copy our vertex data into the buffer memory to our currently bounded VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}