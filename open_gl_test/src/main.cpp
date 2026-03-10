#pragma once
#include "globals.h"

int main()
{
    // window resolution 1920x1080 (full screen for my pc)
    int width= 1920, height= 1080;

    // initialize GLFW: creates the window and context OpenGL
    glfwInit();

    // configure GLFW for OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Create window function needs
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
    // GLAD keeps in track all the openGL functions so the OS can run
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // glfwGetProcAddress defines the correct function based on which OS we are compilling
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // 3D depth effect after loading glad 
    // enables z-axis depth effect for the objects
    glEnable(GL_DEPTH_TEST);


    //  viewport and callback for resize
    //set the size of the rendering window
    glViewport(0, 0, width, height);
    // tell to GLFW to call this function in every window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // position (x, y , z)
    glEnableVertexAttribArray(0);

    // texture coords
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // normal (nx , ny , nz)
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texcoord (u , v)
    glEnableVertexAttribArray(2);

    // Unbind (optional but clean)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // creating a shader class object
    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl" );

    
    // load sphere object (planet.obj) and planet's texture (planet_Quom1200.png) 
    ObjLoader sphere("models/planet.obj", "models/planet_Quom1200.png");

    // load cube's texture in cubeTexture variable to use later when renderind cubes
    unsigned int cubeTexture = loadTexture("models/container.png");


    while (!glfwWindowShouldClose(window))              // entering render loop
    {
        // call processInput function to check the keyboard input if "esc" then closes window
        processInput(window);
        float currentTime = glfwGetTime();              // time since the program started (float)seconds
        float difference = currentTime - lastFrame;     // time difference between currentTime and the time of the last frame
        lastFrame = currentTime;

        if (!isPaused)                      // if scene is moving advance the running time with the time differences 
        {
            runningTime += difference;      // the scene is moving for "runningTime" seconds
        }
        float time = runningTime;           // set time to runningTime 

        int numCubes = 6;                   // number of cubes to draw
        float cubeOrbitRadius = 3.0f;       //  distance from the sphere
        float cubeOrbitSpeed = 1.0f;        //  orbit speed
        float planetOrbitRadius = 4.0f;     // radious of the planet's orbit


        
        glfwGetFramebufferSize(window, &width, &height);    // if the window resizes get width and height
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // clear frame buffer Rendering window+adding color black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //  vector of the planets position
        glm::vec3 planetPos;
        planetPos.x = cos(time) * planetOrbitRadius;        // using cos(t),sin(t) to create a circle while t changes
        planetPos.y = 0.0f;                                 // y-axis remains stable at 0
        planetPos.z = sin(time) * planetOrbitRadius - 4.0f; // adding (- 6) to send it further away from the camera

        // apply shader
        shader.use();
        shader.setVec3("lightPos", planetPos);                  // position of the light is the planet's position
        shader.setVec3("viewPos", glm::vec3(0.0f, 0.0f, 0.0f)); // position of camera at origin before view transform

        ////////// Camera Angles /////////////
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // when pressing up arrow decrease vertical variable
            vertical -= cameraSpeed * difference;

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // when pressing down arrow increase vertical variable
            vertical += cameraSpeed * difference;

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // when pressing left arrow increase horizontal variable
            horizontal += cameraSpeed * difference;

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // when pressing right arrow decrease horizontal variable
            horizontal -= cameraSpeed * difference;
        
        glm::vec3 front;                        // transform spherical to cartesian coordinates 
        front.x = cos(glm::radians(horizontal)) * cos(glm::radians(vertical));
        front.y = sin(glm::radians(vertical));
        front.z = sin(glm::radians(horizontal)) * cos(glm::radians(vertical));

        cameraFront = glm::normalize(front);    // normalizing direction vectors.
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);     // creates a matrix for the camera


        //Perspective matrix: perspective projection like the camera lens. fov = 45°, aspect = width / height, near = 0.1, far = 100.
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f ); // glm::radians makes 45.0f -> rad
        shader.setMat4("projection", projection);

        ////////// Sphere /////////////

        glm::mat4 sphereModel = glm::mat4(1.0f);                                    // Model matrix = position, size, rotation of the model "sphere"
                                                                                    // transformations on the model "sphere Scale -> Rotate -> Translate
        sphereModel = glm::translate(sphereModel, planetPos );                      // bring to the planetPos with calculated at the start of the render loop
        sphereModel = glm::scale(sphereModel, glm::vec3(0.5f));                     // scale it down 
        sphereModel = glm::rotate(sphereModel, time, glm::vec3(0.0f, 1.0f, 0.0f));  // rotate the sphere with the time as input

        // calculate final position using shaders, sending matrices and uniforms to GPU.
        shader.setVec4("ourColor", 0.0f, 0.0f, 0.0f, 1.0f); // sets ourColor to  0.0f, 0.0f, 0.0f, 1.0f
        shader.setMat4("view", view);                       // apply shaders to view
        shader.setMat4("model", sphereModel);               // apply shaders to sphereModel
        shader.setBool("isPlanet", true);                   // send true to isPlanet to use the planets fragment color
        sphere.draw();                                      // draw the planet

        shader.setBool("isPlanet", false);                  // reset isPlanet to draw the cubes with lighting

       
        glBindVertexArray(VAO);                     // bind VAO for the cubes
        glActiveTexture(GL_TEXTURE0);               // activate texture0 position
        glBindTexture(GL_TEXTURE_2D, cubeTexture);  // store cubeTexture in the texture0 position

        for (int i = 0; i < numCubes; i++)
        {
            float angleOffset = i * glm::radians(60.0f);                // each cube is on a different angle 360/6 = 60 radians
            float orbitAngle = time * cubeOrbitSpeed + angleOffset;     // orbit angle changes with time
            float angle = time + i;                                     // different angle for each cube using index "i"
            float selfSpeed = 1.0f + i * 0.3f;                          // rotate cube with different rotation speed for each cube using index "i"

            // vertex offset for final position around the sphere
            glm::vec3 offset;
            offset.x = cos(orbitAngle) * cubeOrbitRadius; // calculate x using orbit angle to be in a different x than the other cubes
            offset.y = 0;
            offset.z = sin(orbitAngle) * cubeOrbitRadius; // calculate x using orbit angle to be in a different z than the other cubes
            glm::vec3 cubePos = planetPos + offset;       // cube's position is planet's position + offset


            // 4x4 model matrix
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePos);                             // transform cube to new cube's position
            model = glm::rotate(model, angle, glm::vec3(0.5f, 1.0f, 0.0f));     // apply rotation cube number "i"
            shader.setMat4("model", model);
            shader.setInt("ourTexture", 0); 

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);    // Swap buffers(back and front when back frame is ready) + poll events
        glfwPollEvents();           
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader.ID);
    glfwTerminate();
    return 0;
}



// called whenever the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// input function (ESC closes the window)
void processInput(GLFWwindow* window)
{
    static bool keyPressed = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !keyPressed)
    {
        isPaused = !isPaused;
        keyPressed = true;
    }

    // when the space is released it sets the keyPressed bool to false so it can reset if the button is pressed again
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        keyPressed = false;
    }
}
