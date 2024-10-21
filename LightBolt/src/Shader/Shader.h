#ifndef SHADER__H
#define SHADER__H

#include <string>

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);

    ~Shader();

    // use/activate the shader
    void Use();

    // utility uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec4f(const std::string& name, float x, float y, float z, float w) const;

    static void CompileShader(unsigned int& ShaderID, const char* ShaderSource, const int GL_ShaderType,
                                char LogBuffer[], const unsigned int BufferSize);

    static void CreateShaderProgram(unsigned int& SProgram, const unsigned int VShader, const unsigned int FShader,
                                    char LogBuffer[], const unsigned int BufferSize);

    static void CheckShaderCompilation(unsigned int Shader, char LogBuffer[], const unsigned int BufferSize);

    static void CheckShaderProgramLinking(unsigned int ShaderProgram, char LogBuffer[], const unsigned int BufferSize);
};

#endif SHADER__H