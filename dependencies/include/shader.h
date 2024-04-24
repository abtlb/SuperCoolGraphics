#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

class Shader
{
public:
    unsigned int shaderProgram;

    Shader(std::string vShaderPath, std::string fShaderPath);
    void useProgram();
};

Shader::Shader(std::string vShaderPath, std::string fShaderPath)
{
    std::ifstream vShaderFile, fShaderFile;
    vShaderFile.exceptions(std::fstream::failbit | std::fstream::badbit);
    fShaderFile.exceptions(std::fstream::failbit | std::fstream::badbit);

    std::stringstream vShaderStream, fShaderStream;
    std::string vShaderStr, fShaderStr;
    try
    {
        vShaderFile.open(vShaderPath);
        fShaderFile.open(fShaderPath);
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vShaderStr = vShaderStream.str();
        fShaderStr = fShaderStream.str();
    }
    catch(const std::fstream::failure& f)
    {
        std::cerr << "Error: creating shader failed, " << f.what() << std::endl;
        return;
    }
    
    const char* vShaderCode = vShaderStr.c_str();
    const char* fShaderCode = fShaderStr.c_str();

    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vShader, 1, &vShaderCode, NULL);
    glShaderSource(fShader, 1, &fShaderCode, NULL);
    glCompileShader(vShader);
    glCompileShader(fShader);

    int success;
     char infoLog[512];
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
        std::cerr << "Error: Vertex shader compilation failed\n" << infoLog << std::endl;
        return;
    }
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        std::cerr << "Error: Fragment shader compilation failed\n" << infoLog << std::endl;
        return;
    }

    shaderProgram = glCreateProgram();//ID
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Error: Shader program linking failed failed\n" << infoLog << std::endl;
        return;
    }
    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

void Shader::useProgram()
{
    glUseProgram(shaderProgram);
}


#endif