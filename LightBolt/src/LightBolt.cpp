#include <glad/glad.h> //glad need to be include before glfw
#include <GLFW/glfw3.h>

#include "Shader/Shader.h"

#include "thirdParty/stb_image.h"

//std
#include <stdexcept>
#include <iostream>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

void Framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

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



    // ______ TEXTURES ______
    std::string textureDir = "..\\Ressources\\Textures\\";

    unsigned int textures[2];
    glGenTextures(2, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Load and generate the texture
    int width, height, nrChannels;
    //Flip the y axis from the image, usually the y = 0 is at the top, but we want it to be at the bottom
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load((textureDir + "WoodenContainer.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        //Args : Tex target, MimMap level, Format of the final tex, width, height, 0 (legacy), format original tex, stored as char, ImageData
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        //Generate MimMap using the currently bounded texture, we can also generate each texture manually
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    glBindTexture(GL_TEXTURE_2D, textures[1]);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load((textureDir + "EmoteAwesomeFace.png").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }


    // ------ SHADERS ------
    std::string shaderDir = "..\\Ressources\\Shaders\\";
    std::string vShaderDir = shaderDir + "Vertex\\";
    std::string fShaderDir = shaderDir + "Fragment\\";

    Shader shaderMultipleColors((vShaderDir + "MovingObjectWithColor.vs").c_str(), (fShaderDir + "SimpleWithColor.fs").c_str());
    Shader shaderTimeColor((vShaderDir + "SimplePos.vs").c_str(), (fShaderDir + "SimpleDrawPosition.fs").c_str());
    Shader shaderSimpleTex((vShaderDir + "SimplePosColTex.vs").c_str(), (fShaderDir + "SimpleWithTexture.fs").c_str());

    // ------ VERTICES DATAS ------

    float vertices1[] = 
    {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    //float vertices[] = 
    //{
    //     0.5f,  0.5f, 0.0f,  // top right
    //     0.5f, -0.5f, 0.0f,  // bottom right
    //    -0.5f, -0.5f, 0.0f,  // bottom left
    //    -0.5f,  0.5f, 0.0f   // top left 
    //};
    unsigned int indices[] = 
    {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // ______________ 1. Preparing to send data to the GPU ______________
    //Vertex Array Object -> we can configure multiple VBO with this object. When we want to draw them, we just bind to this
    //to this VAO, unbind when ended.
    unsigned int VAOs[1]; //Vertex Array Object
    unsigned int VBOs[1]; //Vertex Buffer Object
    unsigned int EBOs[1]; //Element Buffer Object
    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, VBOs);
    glGenBuffers(1, EBOs);

    //After binding VAO, we can bind/configure the corresponding VBO(s) and attribute pointer(s)
    //and then unbind the VAO for later use
    glBindVertexArray(VAOs[0]);

    // ______________ 2. Copy our vertices array in a buffer for OpenGL to use ______________
    //Actions on GL_ARRAY_BUFFER can configure our VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    //Copy our vertex data into the buffer memory to our currently bounded VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    // ______________ 3. Copy our index array in a element buffer for OpenGL to use ______________
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // ______________ 4. then set the vertex attributes pointers ______________
    //Args : Layout location value from vertex shader, vec3 = 3, type of data, normalized the datas ?,
    //memory space between 2 vertex attribute, offset where position data begin 

    //Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // We can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    //Unbind EBO after the unbinding of our VAO -> This EBO will then be set as the EBO of our previous VAO
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    //GL_FILL and GL_LINE to alternate between wireframe mode on/off
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    float time{ 0.f };

    shaderSimpleTex.Use();
    
    //tell OpenGL to which texture unit each shader sampler belongs
    //The uniform "texture2" is now bound to GL_TEXTURE1
    shaderSimpleTex.SetInt("texture2", 1);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        time = glfwGetTime();

        ProcessInput(window);

        //Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderSimpleTex.Use();

        //shaderMultipleColors.SetFloat("offset", sin(time) / 2.0f);

        //We can pass the texture to the shader using this
        //By default, only GL_Tex_0 is active
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        glBindVertexArray(VAOs[0]);
        
        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //Used when we want to render the triangle from an index buffer
        //Args : what we want to draw, number of vertices to draw, type of the indices, offset for the EBO
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //NO EBO is used with this function
        //Args : Type of primitives, starting index of vertex array we want to draw, number of vertices to draw
        //glDrawArrays(GL_TRIANGLES, 0, 3);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, VAOs);
    glDeleteBuffers(1, VBOs);
    glDeleteBuffers(1, EBOs);

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