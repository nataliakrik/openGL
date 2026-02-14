#include <iostream>
#include <glad/glad.h>    // OpenGL function loader
#include <GLFW/glfw3.h>   // Window + input


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "ObjLoader.h"
#include "sphereTexture.h"


// Called whenever the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Input function (ESC closes the window)
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    // window measure
    int width= 1920, height= 1080;
    // Initialize GLFW
    glfwInit();

    // Configure GLFW for OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Create window
    /*
        • width : The desired width, in screen coordinates, of the window.
        • height : The desired height, in screen coordinates, of the window.
        • title : The initial, UTF - 8 encoded window title.
        • monitor : The monitor to use for full screen mode, or NULL for windowed mode.
        • share : The window whose context to share resources with, or NULL to not share resources.
    */
    GLFWwindow* window = glfwCreateWindow(width, height, "Moving Objects", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    // Make context current
    glfwMakeContextCurrent(window);

    // Load GLAD after context is created 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // glfwGetProcAddress defines the correct function based on which OS we are compilling
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // 3D depth effect after loading glad
    glEnable(GL_DEPTH_TEST);


    //  viewport and callback for resize
    //set the size of the rendering window
    glViewport(0, 0, width, height);
    // tell to GLFW to call this function in every window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

     // cube data

    float vertices[] = { // cube
        // front face
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        // back face
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        // left face
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // right face
        0.5f,  0.5f,  0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,

        // top face
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // bottom face
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f
    };

    glm::vec3 cubePositions[] =
    {
        glm::vec3(-3.0f,  0.0f,  -5.0f),
        glm::vec3(0.0f,  3.5f,  -5.0f),
        glm::vec3(0.0f,  -3.5f,  -5.0f),
    };


        
    // initialization of
    // VAO(vertex array object) = instructions on how to read
    // VBO(vertex buffer object) = data of the object
    // EBO(Element Buffer Object) = data for the triagnles
    unsigned int VAO, VBO;

    // getting VAO & VBO id's(address)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO first to store 
    glBindVertexArray(VAO);

    // Bind VBO and send data of vertices array storing it 
    // the GL_STATIC_DRAW is a signal to store data somewhere where you can get it fast because the data will mostly remain the same
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);




    // Tell OpenGL how to read the data
    /*
    * 0 -> starting location of the first point
    * 3 -> each time get 3 numbers
    * GL_FLOAT -> the numbers are float
    * 3 * sizeof(float) -> each point will start at a location that can be found with this
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind (optional but clean)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*
    * τωρα καλοντας
    * glBindVertexArray(VAO)
    * η gpu ξερει πως να χειριστει το αντικειμενο
    */


    // creating a shader class object
    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl" );

    
    // sphere
    ObjLoader sphere("models/planet.obj");
    unsigned int planetTexture = loadTexture("models/planet_Quom1200.png");



    // entering render loop
    while (!glfwWindowShouldClose(window))
    {
        // if esc i pressed window closes
        processInput(window);
        // if the window resizes get width and height
        glfwGetFramebufferSize(window, &width, &height);

        // Rendering clear window+adding color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




        // apply shader
        shader.use();
        float timeValue = (float)glfwGetTime();

        // move camera backwards (so we can see cube)
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

        // perspective projection
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)width / (float)height,
            0.1f,
            100.0f
        );
        shader.setMat4("projection", projection);


        ////////// Sphere /////////////
        glm::mat4 sphereModel = glm::mat4(1.0f);

        // center
        sphereModel = glm::translate(sphereModel, glm::vec3(0.0f, 0.0f, -5.0f));

        // scale it down (OBJ models are often huge)
        sphereModel = glm::scale(sphereModel, glm::vec3(0.5f));

        // rotate like earth
        sphereModel = glm::rotate(sphereModel, timeValue, glm::vec3(0.0f, 1.0f, 0.0f));

        shader.setMat4("model", sphereModel);

        // set sphere color (white/yellow)
        shader.setVec4("ourColor", 1.0f, 1.0f, 0.8f, 1.0f);

        sphere.draw();



        // Draw triangle
        shader.setVec4("ourColor", 0.0f, 1.0f, 0.0f, 1.0f);
        shader.setMat4("view", view);
        glBindVertexArray(VAO);


        for (int i = 0; i <3; i++)
        {

            // 3D effect
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = (float)glfwGetTime() + i;
            // rotate cube
            model = glm::rotate(model, angle, glm::vec3(0.5f, 1.0f, 0.0f));

            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Swap buffers(back and front when back frame is ready) + poll events
        glfwSwapBuffers(window);
        // check if the user pressed esc to close window
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader.ID);
    glfwTerminate();

    return 0;
}

