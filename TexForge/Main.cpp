#include<iostream>

// Dear ImGui core
#include <imgui.h>

// ImGui integrations:
// GLFW handles window/input
// OpenGL3 handles rendering the UI
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


// OpenGL function loader
#include<glad/glad.h>

// Window and input library
#include<GLFW/glfw3.h>


// Image loading library
#include<stb/stb_image.h>


#include <filesystem>


// GLM mathematics library
// Used for vectors, matrices and 3D transformations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// TexForge engine classes
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include"Camera.h"



// Vertex data
//
// Each vertex contains:
//
// Position:  x,y,z
// Color:     r,g,b
// Texture:   u,v
//
// Layout:
// [position][color][uv]
//
GLfloat vertices[] =
{
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f
};



// Instead of storing duplicate vertices,
// an EBO stores the order in which vertices are used.
//
// This draws two triangles:
//
// Triangle 1: 0 -> 2 -> 1
// Triangle 2: 0 -> 3 -> 2
//
GLuint indices[] =
{
	0,2,1,
	0,3,2
};



int main()
{

	// ----------------------------------------
	// Initialize GLFW
	// ----------------------------------------

	glfwInit();



	// Request OpenGL 3.3 Core profile.
	//
	// Core profile removes old fixed pipeline features
	// and forces modern OpenGL usage.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,
		GLFW_OPENGL_CORE_PROFILE);



	// Create the application window.
	GLFWwindow* window = glfwCreateWindow(
		800,
		800,
		"TexForge",
		NULL,
		NULL
	);



	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}



	// Makes this window the active OpenGL context.
	glfwMakeContextCurrent(window);



	// Load OpenGL functions.
	//
	// OpenGL functions are not directly available from the OS.
	// GLAD loads the correct function pointers.
	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}



	// Define the area OpenGL renders into.
	glViewport(
		0,
		0,
		800,
		800
	);



	// Enable depth buffering.
	//
	// Required for proper 3D rendering:
	// closer objects hide objects behind them.
	glEnable(GL_DEPTH_TEST);





	// ----------------------------------------
	// Initialize ImGui
	// ----------------------------------------


	IMGUI_CHECKVERSION();


	// Creates ImGui internal state.
	ImGui::CreateContext();



	// Access ImGui settings.
	ImGuiIO& io = ImGui::GetIO();



	// Default dark editor theme.
	ImGui::StyleColorsDark();



	// Connect ImGui to GLFW.
	// Handles keyboard, mouse and window input.
	ImGui_ImplGlfw_InitForOpenGL(
		window,
		true
	);



	// Connect ImGui to OpenGL.
	// This creates the GPU resources needed
	// to draw the interface.
	ImGui_ImplOpenGL3_Init(
		"#version 330"
	);





	// ----------------------------------------
	// Create rendering objects
	// ----------------------------------------


	// Load vertex and fragment shaders.
	//
	// Vertex shader:
	// transforms 3D coordinates
	//
	// Fragment shader:
	// decides pixel colors
	//
	Shader shaderProgram(
		"vertex.vert",
		"fragment.frag"
	);



	// VAO remembers the layout of vertex data.
	//
	// It stores information like:
	//
	// location 0 -> position
	// location 1 -> color
	// location 2 -> texture coordinates
	//
	VAO VAO1;
	VAO1.Bind();



	// Upload vertex data to GPU.
	VBO VBO1(
		vertices,
		sizeof(vertices)
	);



	// Upload index data to GPU.
	EBO EBO1(
		indices,
		sizeof(indices)
	);




	// Tell OpenGL how the vertex data is structured.

	// Position attribute
	VAO1.LinkAttrib(
		VBO1,
		0,
		3,
		GL_FLOAT,
		8 * sizeof(float),
		(void*)0
	);


	// Color attribute
	VAO1.LinkAttrib(
		VBO1,
		1,
		3,
		GL_FLOAT,
		8 * sizeof(float),
		(void*)(3 * sizeof(float))
	);



	// Texture coordinate attribute
	VAO1.LinkAttrib(
		VBO1,
		2,
		2,
		GL_FLOAT,
		8 * sizeof(float),
		(void*)(6 * sizeof(float))
	);



	// Prevent accidental modification.
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();




	// Shader uniform used for testing scaling.
	GLuint uniID =
		glGetUniformLocation(
			shaderProgram.ID,
			"scale"
		);




	// Load texture into GPU memory.
	Texture testImage(
		"test_image.png",
		GL_TEXTURE_2D,
		GL_TEXTURE0,
		GL_RGBA,
		GL_UNSIGNED_BYTE
	);



	testImage.texUnit(
		shaderProgram,
		"tex0",
		0
	);




	// Camera controls the view matrix.
	//
	// Position:
	// x = 0
	// y = 0
	// z = 2
	//
	Camera camera(
		800,
		800,
		glm::vec3(0, 0, 2)
	);



	float brushSize = 25.0f;





	// ----------------------------------------
	// Main application loop
	// ----------------------------------------

	while (!glfwWindowShouldClose(window))
	{

		// Process keyboard/mouse/window events.
		glfwPollEvents();




		// Clear previous frame.
		glClearColor(
			0.07f,
			0.13f,
			0.17f,
			1.0f
		);


		glClear(
			GL_COLOR_BUFFER_BIT |
			GL_DEPTH_BUFFER_BIT
		);





		// ----------------------------------------
		// Render 3D scene
		// ----------------------------------------


		shaderProgram.Activate();


		// Update camera movement.
		camera.Inputs(window);



		// Send camera matrices to GPU.
		camera.Matrix(
			45.0f,
			0.1f,
			100.0f,
			shaderProgram,
			"camMatrix"
		);



		glUniform1f(
			uniID,
			0.5f
		);



		VAO1.Bind();

		testImage.Bind();



		// Draw indexed triangles.
		glDrawElements(
			GL_TRIANGLES,
			sizeof(indices) / sizeof(GLuint),
			GL_UNSIGNED_INT,
			0
		);





		// ----------------------------------------
		// Build ImGui interface
		// ----------------------------------------

		// Start a new UI frame.
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();




		ImGui::Begin(
			"TexForge"
		);



		ImGui::Text(
			"FPS: %.1f",
			io.Framerate
		);



		ImGui::SliderFloat(
			"Brush Size",
			&brushSize,
			1.0f,
			100.0f
		);



		if (ImGui::Button("Save Project"))
		{
			std::cout << "Saving...\n";
		}



		ImGui::End();





		// Convert ImGui commands into OpenGL draw calls.
		ImGui::Render();


		ImGui_ImplOpenGL3_RenderDrawData(
			ImGui::GetDrawData()
		);





		// Swap back buffer to screen.
		glfwSwapBuffers(window);

	}





	// ----------------------------------------
	// Shutdown
	// ----------------------------------------


	ImGui_ImplOpenGL3_Shutdown();

	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();



	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();

	shaderProgram.Delete();



	glfwDestroyWindow(window);

	glfwTerminate();



	return 0;
}