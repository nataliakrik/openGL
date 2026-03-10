#include "ObjLoader.h"
#include "tiny_obj_loader.h"
#include <iostream>

ObjLoader::ObjLoader(const std::string& path, const char* texturePath)
{
    // loads planet's texture
    planetTexture = loadTexture(texturePath);
    
    // tiny obj loader loads data in 
    // attrib.vertices  attrib.normals  attrib.texcoords 
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn, err;

    // tries to load object
    bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }
    if (!err.empty()) {
        std::cout << "ERR: " << err << std::endl;
    }

    if (!ok)
    {
        std::cout << "Failed to load OBJ: " << path << std::endl;
        return;
    }

    // position (3 floats) + normal (3 floats) + textcoord (2 floats)
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int indexOffset = 0;

    for (const auto& shape : shapes)
    {
        for (const auto& idx : shape.mesh.indices)
        {
            float vx = attrib.vertices[3 * idx.vertex_index + 0];
            float vy = attrib.vertices[3 * idx.vertex_index + 1];
            float vz = attrib.vertices[3 * idx.vertex_index + 2];

            float nx = 0.0f, ny = 0.0f, nz = 0.0f;
            if (idx.normal_index >= 0)
            {
                nx = attrib.normals[3 * idx.normal_index + 0];
                ny = attrib.normals[3 * idx.normal_index + 1];
                nz = attrib.normals[3 * idx.normal_index + 2];
            }

            float tx = 0.0f, ty = 0.0f;
            if (idx.texcoord_index >= 0)
            {
                tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                ty = attrib.texcoords[2 * idx.texcoord_index + 1];
            }

            // each vertex representaion is { vx vy vz nx ny nz tx ty }

            vertices.push_back(vx);
            vertices.push_back(vy);
            vertices.push_back(vz);

            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);

            vertices.push_back(tx);
            vertices.push_back(ty);


            indices.push_back(indexOffset);
            indexOffset++;
        }
    }

    indexCount = (int)indices.size();

    // initialize VAO ,VBO ,EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW);

    // position x y z coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normals 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texcoord texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void ObjLoader::draw()
{
    glBindVertexArray(VAO);
    // activates GL_TEXTURE0 and the bind the planetTexture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, planetTexture);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
