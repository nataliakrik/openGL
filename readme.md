# Graphics Homework openGL
Natalia Krikelli

**Course:**  Graphics I

**Department:** Informatics & Telecommunications

**Semester:** Fall 2025

## About the project

This openGL app creates a rotating planet that is in orbit. Around the planet there are 6 rotating cubes that are in orbit around the planet.  
The planet's position is a light source that light up the faces of the cubes that appear closer to the planet.

Interactions with the user:

  - press "esc" on the keyboard to exit window
  - press "space" on the keyboard to pause the animation
  - move the angle of the  camera on the x and y axis using the arrows (up,down , left, right)


project structure:

	- include folder contains the libraries that were used as well as my class definitions
	- models  folder contains the helper files that were given texture images , planet.obj , planet.mt
	- shaders folder contains the fragment.glsl and the vertex.glsl files
	- src folder contains source code files and main function
	- glad.c source file
	- this readme.md file

------------------------------------------------------------------------------

## Project Description

### About the main

step by step description of the main

1. Window and Context setup
			
	-> Initializes GLFW and creates a 1920×1080 window
    -> Loads GLAD to access OpenGL functions
    -> Enables depth testing

2. VAO and VBO
			
    -> Initializes and binds VAO and VBO for the cube
    -> Loads GLAD to access OpenGL functions
    -> Defines vertex attributes (position, normal,texture coordinates)

3. Shader and Object Loading
	
	-> Creates a `Shader` object loading vertex and fragment code files
    -> Loads the planet OBJ with `ObjLoader` and its texture
    -> Loads cube textures

4. Render Loop

    -> Checks user input and handles for camera movement
    -> Updates time depending is the screen is paused or not
    -> Sets camera `view` and `projection` matrices.
    -> Draws planet and cubes
    -> Swaps buffers and polls events on each frame

5. Cleanup

    -> Deletes VAO, VBO, and shader program.
    -> Terminates GLFW.


------------------------------------------------------------------------------

### About the planet

The planet was build with the help of the "planet.obj" file in the models folder.
The ObjLoader constructorof the ObjLoader class is responsible for loading the "planet.obj" file for rendering in OpenGL.
Using the TinyObjLoader library to parse the model file and extract 
	-> Vertex positions
	-> Normals
	-> Texture coordinates
	-> Face indices

The obj files store these attributes separately, so the loader builds them into new vertex format
{position_coords (3) | normals_coords (3) | tex_coords (2)}

Then the data is then uploaded to the GPU using the usuals VAO , VBO , EBO  
VBO and EBO store vertices and indeces
while VAO store instruction for the GPU

Then the function ObjLoader.draw() draws the planet 
	- calls planet's VAO , VBO, EBO
	- applying the planet texture
	- draws the sphere


------------------------------------------------------------------------------


### About the Shader class


This class is responsible for loading, compiling, and linking GLSL shader programs in OpenGL.

- The constructor of the class:

	1. Reads both vertex and fragment shaders source code from external files
	2. Compiles each shader
	3. Checks for compilation errors and prints error messages
	4. Links shaders into a single shader program
	5. Checks for linking errors and prints error messages
	6. Deletes the individual shader objects after linking

	The final shader program is stored in the class member with its own ID.


- The use() function of the class activates the shader program.

- Functions to set uniform variables  in the shader:

	1. setMat4 : sends 4x4 transformation matrices  ex. model, view, projection
	2. setVec3 : sends 3D vectors
	3. setVec4 : sends 4D vectors
	4. setInt  : sends integer values
	5. setBool : sends boolean values



----------------------------------------------------------------------------------

### About the framebuffer size callback function and the process input function

framebuffer size callback:

	Adjusts the viewport when the window is resized to make sure objects appear correctly with the new width and height.

processInput:
	
	Handles user input:

	- ESC closes the window.

	- SPACE toggles the scene’s pause state 


----------------------------------------------------------------------------------


### About the texture function

The loadTexture function loads an image file and creates a texture to apply to objects.
Using the "stb_image.h" library to read the image from disk and extract
	-> width and height of the image
	-> color channel based on the channels the appropriate OpenGL format is selected
	-> pixel data

Then the data gets uploaded to GPU and is ready to render on objects using the generated texture ID.


----------------------------------------------------------------------------------

