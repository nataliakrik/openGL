#include "Shader.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <fstream>

Shader::Shader(const char* VerteShaderString, const char* fragmentShaderString)
{
    // open Vertex and Fragment Files store data in a stringstream type and transform the ro const char* type 
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    vertexFile.open(VerteShaderString);
    fragmentFile.open(fragmentShaderString);


    std::stringstream vertexData, fragmentData;

    vertexData << vertexFile.rdbuf();
    fragmentData << fragmentFile.rdbuf();

    vertexFile.close();
    fragmentFile.close();

    vertexCode = vertexData.str();
    fragmentCode = fragmentData.str();


    const char* vertex = vertexCode.c_str();
    const char* fragment = fragmentCode.c_str();

    // variable to store error code (if not true then error)
    int success;
    char infoLog[512];
    /////////// Vertex shader /////////////
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // create empty shader
    glShaderSource(vertexShader, 1, &vertex, NULL); // load glsl text 
    glCompileShader(vertexShader); // compilation of glsl

    //Check if compilation was successful :
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /////////// Fragment shader /////////////
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // create empty shader
    glShaderSource(fragmentShader, 1, &fragment, NULL); // load glsl text 
    glCompileShader(fragmentShader); // compilation of glsl

    //Check if compilation was successful:
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    //////////// Linking ////////////////
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // check for linking errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }


    ////////// Deletion /////////////
    // Delete shaders after linking because we created shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
    int colorLoc = glGetUniformLocation(ID, "ourColor");
    glUniform4f(colorLoc, x, y, z, w);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat)
{
    int loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setInt(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value)
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);

}

void Shader::setBool(const std::string& name, bool value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
