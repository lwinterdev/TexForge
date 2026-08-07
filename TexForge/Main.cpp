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
#include"Texture.h"
#include"Camera.h"
#include "assets/Model.h"
#include "assets/ModelImporter.h"
#include "painting/PaintTexture.h"
#include "painting/Brush.h"
#include "engine/RayPicker.h"

void Paint(Brush brush, PaintTexture& paintTexture)
{
	brush.Paint(
		paintTexture,
		1024,
		1024
	);

	paintTexture.Upload();
}



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

	ModelImporter importer;


	Model model =
		importer.Load("./testcube.obj");

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


	PaintTexture paintTexture(
		2048,
		2048,
		testImage.ID
	);

	Brush brush;

	brush.color =
		glm::vec4(
			1.0f,
			0.0f,
			0.0f,
			1.0f
		);





	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();


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


		//----------------------------
		// Camera
		//----------------------------

		camera.Inputs(window);

		camera.Matrix(
			shaderProgram,
			"camMatrix"
		);


		//----------------------------
		// Render model
		//----------------------------

		shaderProgram.Activate();

		model.Draw(shaderProgram);



		//----------------------------
		// Mouse position
		//----------------------------

		double mouseX;
		double mouseY;

		glfwGetCursorPos(
			window,
			&mouseX,
			&mouseY
		);



		//----------------------------
		// Picking / painting
		//----------------------------

		if (glfwGetMouseButton(
			window,
			GLFW_MOUSE_BUTTON_RIGHT
		) == GLFW_PRESS)
		{

			Ray ray =
				RayPicker::ScreenPointToRay(
					mouseX,
					mouseY,
					800,
					800,
					camera
				);


			RaycastHit hit =
				RayPicker::Raycast(
					ray,
					model
				);


			if (hit.hit)
			{
				std::cout
					<< "Position: "
					<< hit.position.x << ", "
					<< hit.position.y << ", "
					<< hit.position.z
					<< "\n";

				std::cout
					<< "UV: "
					<< hit.uv.x
					<< ", "
					<< hit.uv.y
					<< "\n\n";

				int x =
					hit.uv.x *
					paintTexture.width;


				int y =
					(1.0f - hit.uv.y) *
					paintTexture.height;


				brush.Paint(
					paintTexture,
					x,
					y
				);
			}
		}



		//----------------------------
		// ImGui
		//----------------------------

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("TexForge");


		ImGui::Text(
			"FPS: %.1f",
			io.Framerate
		);


		ImGui::SliderFloat(
			"Brush Size",
			&brush.radius,
			1.0f,
			100.0f
		);


		if (ImGui::Button("Save Project"))
		{
			std::cout << "Saving...\n";
		}


		ImGui::End();



		//----------------------------
		// Brush cursor
		//----------------------------

		ImDrawList* drawList =
			ImGui::GetForegroundDrawList();


		drawList->AddCircle(
			ImVec2(mouseX, mouseY),
			brush.radius,
			IM_COL32(255, 255, 255, 255)
		);



		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(
			ImGui::GetDrawData()
		);


		glfwSwapBuffers(window);
	}


	// ----------------------------------------
	// Shutdown
	// ----------------------------------------
	ImGui_ImplOpenGL3_Shutdown();

	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();


	shaderProgram.Delete();



	glfwDestroyWindow(window);

	glfwTerminate();



	return 0;
}