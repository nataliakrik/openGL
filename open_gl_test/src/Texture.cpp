#include <iostream>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int loadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;

    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0); // loads the image to memory returns pixel data to data

    if (data)
    {
        // detect image color format to send to gpu
        GLenum format = GL_RGB;
        if (nrChannels == 1) format = GL_RED;
        if (nrChannels == 3) format = GL_RGB;
        if (nrChannels == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);

        /*parameters 
        (2D texture| mipmap level| internal format in GPU| image size| border (always 0)| format of input data| each color value is 0–255| actual pixel data)*/
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); // sends the image data to the GPU
        glGenerateMipmap(GL_TEXTURE_2D);        //creates smaller versions of the texture ex. when objects move far away

        // texture settings  texture coordinates [0,1] 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // smoother image when we magnify or the image gets smaller
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data); // free memory cause its in the GPU
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}