#pragma once
#include <iostream>
#include <glad/glad.h>    // OpenGL function loader
#include <GLFW/glfw3.h>   // Window + input

#include <glm/glm.hpp> // vec3, vec4 {points , address in space}, mat4 {4x4 matrix} data types
#include <glm/gtc/matrix_transform.hpp> // transform functions like translate, rotate, scale, perspective, lookAt.
#include <glm/gtc/type_ptr.hpp> // Μετατρέπει glm::mat4 ή vec σε pointer (float*) για να περάσει στην OpenGL shader.


#include "Shader.h"
#include "ObjLoader.h"
#include "Texture.h"

// Calculate time

// bool isPaused = false by default. Changes to true when screen is paused.
bool isPaused = false;
float runningTime = 0.0f;
float lastFrame = 0.0f;


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);     // camera starting position
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);   // camera points at camera front faces -z 
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);       // camera upper limit top view

float horizontal = -90.0f;      // left/right when we do a Y axis rotation
float vertical = 0.0f;          // up/down    when we do a X axis rotation

float cameraSpeed = 60.0f;      // degrees per second


/*
* @brief 
* 6 vertices for each face of the cube (2 triangles) 
* 
*  Each vector is represented like this : { x y z nx ny nz u v }
* 
* { x y z } position
* 
* { nx ny nz } light normals
* 
* { u v } texture coordinates
*/
float vertices[] = { 
    // x     y      z     nx    ny    nz    u     v
    // front face
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    // back face
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,-1.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,-1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f,-1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f,-1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f,-1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,

    // left face
    -0.5f,  0.5f,  0.5f,-1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,-1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,-1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,-1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

    // right face
    0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    // top face
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

    // bottom face
    -0.5f, -0.5f, -0.5f, 0.0f,-1.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f,-1.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f,-1.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f,-1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,-1.0f, 0.0f, 0.0f, 0.0f
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// input function (ESC closes the window)
void processInput(GLFWwindow* window);