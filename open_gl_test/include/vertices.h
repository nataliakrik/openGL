#pragma once

// Calculate time

// bool isPaused = false by default. Changes to true when screen is paused.
bool isPaused = false;
float runningTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 15.0f);     // camera starting position
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);   // 
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;  // left/right (Y axis rotation)
float pitch = 0.0f;   // up/down (X axis rotation)

float cameraSpeed = 60.0f; // degrees per second


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