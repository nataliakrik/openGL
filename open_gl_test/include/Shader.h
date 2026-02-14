#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* VerteShaderString, const char* fragmentShaderString);

	void use();

	void setVec4(const std::string& name, float x, float y, float z, float w);
	void setMat4(const std::string& name, const glm::mat4& mat);
};

