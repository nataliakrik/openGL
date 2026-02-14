#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>


class ObjLoader
{
public:
    ObjLoader(const std::string& path);
    void draw();

private:
    unsigned int VAO, VBO, EBO;
    int indexCount;
};

