#include <glad/glad.h> //glad need to be include before glfw
#include <GLFW/glfw3.h>

//std
#include <stdexcept>
#include <iostream>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

const char* vertexShaderSource = "#version 460 core\n"
"layout(location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

void Framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void CheckShaderCompilation(unsigned int Shader, char LogBuffer[], unsigned int BufferSize);
void CheckShaderProgramLinking(unsigned int ShaderProgram, char LogBuffer[], unsigned int BufferSize);

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


    // ______________ 0. Compiling shaders and create shader program ______________
    //------ VERTEX SHADER ------
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Arg : shader, how many strings to use, source code,
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    const unsigned int infoLogSize = 512;
    char infoLog[infoLogSize];
    CheckShaderCompilation(vertexShader, infoLog, infoLogSize);

    //------ FRAGMENT SHADER ------
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    CheckShaderCompilation(fragmentShader, infoLog, infoLogSize);

    //------ SHADER PROGRAM -> Final object linking all parts of the shader we want to create ------
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    CheckShaderProgramLinking(shaderProgram, infoLog, infoLogSize);

    //We can delete the shaders used to create our shaderProgram to free memory
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    //Set up vertex data (and buffer(s)) and configure vertex attributes
    //x,y,z format
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // ______________ 1. Preparing to send data to the GPU ______________
    //Vertex Array Object
    unsigned int VAO;
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //After binding VAO, we can bind/configure the corresponding VBO(s) and attribute pointer(s)
    //and then unbind the VAO for later use
    glBindVertexArray(VAO);    

    // ______________ 2. Copy our vertices array in a buffer for OpenGL to use ______________
    //Actions on GL_ARRAY_BUFFER can configure our VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Copy our vertex data into the buffer memory to our currently bounded VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    
    // ______________ 3. then set the vertex attributes pointers ______________
    //Args : Layout location value from vertex shader, vec3 = 3, type of data, normalized the datas ?,
    //memory space between 2 vertex attribute, offset where position data begin 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's
    // bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // We can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        //Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //Args : Type of primitives, starting index of vertex array we want to draw, number of vertices to draw
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return EXIT_SUCCESS;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void CheckShaderCompilation(unsigned int Shader, char LogBuffer[], unsigned int BufferSize)
{
    int success;
    memset(LogBuffer, 0, sizeof(LogBuffer));  // Reset LogBuffer
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(Shader, BufferSize, NULL, LogBuffer);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << LogBuffer << std::endl;
    }
}

void CheckShaderProgramLinking(unsigned int ShaderProgram, char LogBuffer[], unsigned int BufferSize)
{
    int success;
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(ShaderProgram, BufferSize, NULL, LogBuffer);
        std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" << LogBuffer << std::endl;
    }
}