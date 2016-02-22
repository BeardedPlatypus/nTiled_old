#include <iostream>

#include <imgui.h>
#include "gui/imgui_impl_glfw_gl3.h"

// GLAD
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>

#include "state\State.h"
#include "gui\GuiManager.h"
#include "pipeline\PipelineManager.h"
#include "world\ObjectConstructors.h"

#include "pipeline\shaders\forward-shading\BasicForwardLightShader.h"
#include "pipeline\shaders\deferred-shading\BasicDeferredLightShader.h"

// TODO change this to parameter
#define SCENE_PATH "C:/Users/Monthy/Documents/projects/thesis/scenes/scene-definitions/light_test2/scene.json"
//#define SCENE_PATH "C:/Users/Monthy/Documents/projects/thesis/scenes/scene-definitions/solid_test3/scene.json"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Loading state
	// ------------------------------------------------------------------------
	std::cout << "Loading program state." << std::endl;
	nTiled_state::State state = nTiled_state::State(SCENE_PATH);

    // Loading OpenGL and window
	// ------------------------------------------------------------------------
	std::cout << "Starting GLFW context, OpenGL 4.4" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(state.viewport.x, 
		                                  state.viewport.y, 
		                                  "nTiled", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Load GLAD
	// ------------------------------------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "loaded opengl " << glGetString(GL_VERSION) << std::endl;

	// Define the viewport dimensions
	glViewport(0, 0, state.viewport.x, state.viewport.y);

	// SetUp program elements
	// ------------------------------------------------------------------------
	// Set up GUI manager
	nTiled_gui::GuiManager gui_manager = nTiled_gui::GuiManager(state);
	gui_manager.init(*window);

	nTiled_pipeline::PipelineManager pipeline_manager =
		nTiled_pipeline::PipelineManager(state);
	
	// initialise pipeline and pipeline manager.
	pipeline_manager.init();

	ImVec4 clear_color = gui_manager.getClearColor();
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		gui_manager.update();
		clear_color = gui_manager.getClearColor();

		// Render
		// Clear the colorbuffer
		glClearColor(0, 0, 0, 1);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render elements
		pipeline_manager.render();
		gui_manager.render();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminates GLFW, clearing any resources allocated by GLFW.
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();

	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}