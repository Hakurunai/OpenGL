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

const char* fragShaderSourceOrange = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragShaderSourceYellow = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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
    unsigned int orangeFrag;
    orangeFrag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(orangeFrag, 1, &fragShaderSourceOrange, NULL);
    glCompileShader(orangeFrag);

    CheckShaderCompilation(orangeFrag, infoLog, infoLogSize);

    unsigned int yellowFrag;
    yellowFrag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(yellowFrag, 1, &fragShaderSourceYellow, NULL);
    glCompileShader(yellowFrag);

    CheckShaderCompilation(yellowFrag, infoLog, infoLogSize);

    //------ SHADER PROGRAM -> Final object linking all parts of the shader we want to create ------
    unsigned int shaderProgramOrange;
    shaderProgramOrange = glCreateProgram();
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, orangeFrag);
    glLinkProgram(shaderProgramOrange);

    CheckShaderProgramLinking(shaderProgramOrange, infoLog, infoLogSize);


    // ANOTHER SHADER PROGRAM
    unsigned int shaderProgramYellow;
    shaderProgramYellow = glCreateProgram();
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, yellowFrag);
    glLinkProgram(shaderProgramYellow);

    CheckShaderProgramLinking(shaderProgramYellow, infoLog, infoLogSize);

    //We can delete the shaders used to create our shaderProgram to free memory
    glDeleteShader(vertexShader);
    glDeleteShader(orangeFrag);
    glDeleteShader(yellowFrag);


    //Set up vertex data (and buffer(s)) and configure vertex attributes
    //x,y,z format
    float vertices1[] =
    {
        // first triangle
       -0.9f, -0.5f, 0.0f,  // left 
       -0.0f, -0.5f, 0.0f,  // right
       -0.45f, 0.5f, 0.0f   // top 
    };

    float vertices2[]
    {
        // second triangle
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top 
    };
    //float vertices[] = 
    //{
    //     0.5f,  0.5f, 0.0f,  // top right
    //     0.5f, -0.5f, 0.0f,  // bottom right
    //    -0.5f, -0.5f, 0.0f,  // bottom left
    //    -0.5f,  0.5f, 0.0f   // top left 
    //};
    //unsigned int indices[] = 
    //{  // note that we start from 0!
    //    0, 1, 3,   // first triangle
    //    1, 2, 3    // second triangle
    //};

    // ______________ 1. Preparing to send data to the GPU ______________
    //Vertex Array Object -> we can configure multiple VBO with this object. When we want to draw them, we just bind to this
    //to this VAO, unbind when ended.
    unsigned int VAOs[2]; //Vertex Array Object
    unsigned int VBOs[2]; //Vertex Buffer Object
    //unsigned int EBO; //Element Buffer Object
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    //glGenBuffers(1, &EBO);

    //After binding VAO, we can bind/configure the corresponding VBO(s) and attribute pointer(s)
    //and then unbind the VAO for later use
    glBindVertexArray(VAOs[0]);

    // ______________ 2. Copy our vertices array in a buffer for OpenGL to use ______________
    //Actions on GL_ARRAY_BUFFER can configure our VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    //Copy our vertex data into the buffer memory to our currently bounded VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    // ______________ 3. Copy our index array in a element buffer for OpenGL to use ______________
    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
    
    // ______________ 4. then set the vertex attributes pointers ______________
    //Args : Layout location value from vertex shader, vec3 = 3, type of data, normalized the datas ?,
    //memory space between 2 vertex attribute, offset where position data begin 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    // We can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    //Unbind EBO after the unbinding of our VAO -> This EBO will then be set as the EBO of our previous VAO
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    glBindVertexArray(VAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    //GL_FILL and GL_LINE to alternate between wireframe mode on/off
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        //Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgramOrange);
        glBindVertexArray(VAOs[0]);

        //Used when we want to render the triangle from an index buffer
        //Args : what we want to draw, number of vertices to draw, type of the indices, offset for the EBO
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //NO EBO is used with this function
        ////Args : Type of primitives, starting index of vertex array we want to draw, number of vertices to draw
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgramOrange);

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