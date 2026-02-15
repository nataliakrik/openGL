#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include "Texture.h"



class ObjLoader
{
public:
    ObjLoader(const std::string& path, const char* texturePath);
    void draw();

    // planetTexture
    unsigned int planetTexture;

private:
    unsigned int VAO, VBO, EBO;
    int indexCount;
};

