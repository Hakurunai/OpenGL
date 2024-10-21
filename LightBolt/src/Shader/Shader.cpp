#include "Shader.h"

#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();


    // 2. compile shaders
    unsigned int vertex, fragment;
    const unsigned int logSize{ 512 };
    char infoLog[logSize];

    CompileShader(vertex, vShaderCode, GL_VERTEX_SHADER, infoLog, logSize);
    CompileShader(fragment, fShaderCode, GL_FRAGMENT_SHADER, infoLog, logSize);

    //ShaderProgram
    CreateShaderProgram(ID, vertex, fragment, infoLog, logSize);

    //delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::Use()
{
    glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec4f(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

//GL_ShaderType exemple : GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
void Shader::CompileShader(unsigned int& ShaderID, const char* ShaderSource, const int GL_ShaderType, char LogBuffer[],
                           const unsigned int BufferSize)
{
    ShaderID = glCreateShader(GL_ShaderType);
    glShaderSource(ShaderID, 1, &ShaderSource, NULL);
    glCompileShader(ShaderID);
    CheckShaderCompilation(ShaderID, LogBuffer, BufferSize);
}

//------ SHADER PROGRAM -> Final object linking all parts of the shader we want to create ------
void Shader::CreateShaderProgram(unsigned int& SProgram, const unsigned int VShader, const unsigned int FShader, 
                                 char LogBuffer[], const unsigned int BufferSize)
{
    SProgram = glCreateProgram();
    glAttachShader(SProgram, VShader);
    glAttachShader(SProgram, FShader);
    glLinkProgram(SProgram);
    CheckShaderProgramLinking(SProgram, LogBuffer, BufferSize);
}

void Shader::CheckShaderCompilation(unsigned int Shader, char LogBuffer[], const unsigned int BufferSize)
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

void Shader::CheckShaderProgramLinking(unsigned int ShaderProgram, char LogBuffer[], const unsigned int BufferSize)
{
    int success;
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ShaderProgram, BufferSize, NULL, LogBuffer);
        std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" << LogBuffer << std::endl;
    }
}
