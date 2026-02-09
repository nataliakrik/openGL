#include <iostream>
#include <glad/glad.h>    // OpenGL function loader
#include <GLFW/glfw3.h>   // Window + input


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

// ======================= SHADERS =======================

// Vertex shader source
const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

// Fragment shader source
const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    // --> 1) Initialize GLFW
        glfwInit();

    // --> 2) Configure GLFW for OpenGL 3.3 Core Profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // --> 3) Create window
        /*
            • width : The desired width, in screen coordinates, of the window.
            • height : The desired height, in screen coordinates, of the window.
            • title : The initial, UTF - 8 encoded window title.
            • monitor : The monitor to use for full screen mode, or NULL for windowed mode.
            • share : The window whose context to share resources with, or NULL to not share resources.
        */
        GLFWwindow* window = glfwCreateWindow(800, 600, "Moving Objects", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window\n";
            glfwTerminate();
            return -1;
        }

    // --> 4) Make context current
        glfwMakeContextCurrent(window);

    // --> 5) Load GLAD after context is created 
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            // glfwGetProcAddress defines the correct function based on which OS we are compilling
            std::cout << "Failed to initialize GLAD\n";
            return -1;
        }

    //  --> 6) Set viewport and callback for resize
        //set the size of the rendering window
        glViewport(0, 0, 800, 600);
        // tell to GLFW to call this function in every window resize
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // ======================= TRIANGLE DATA =======================
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        // VAO + VBO
        unsigned int VAO, VBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // Bind VAO first
        glBindVertexArray(VAO);

        // Bind VBO and send data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Tell OpenGL how to read the data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind (optional but clean)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // ======================= SHADER COMPILATION =======================

        // Vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // Fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Shader program
        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Delete shaders after linking
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);


    // --> 7) Render loop
        while (!glfwWindowShouldClose(window))
        {
            // Input
            processInput(window);

            // Rendering
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Draw triangle
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // Swap buffers + poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    // --> 8) Cleanup
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
        glfwTerminate();

    return 0;
}

