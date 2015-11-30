#include <iostream>

#include <imgui.h>
#include "gui/imgui_impl_glfw_gl3.h"

// GLAD
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>

#include "gui\GuiManager.h"
#include "pipeline\PipelineManager.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;


// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 4.5" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "nTiled", NULL, NULL);
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// Construct a camera
	glm::vec3 camera_eye = glm::vec3(0.0, 1.0, 4.0);
	glm::vec3 camera_center = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 camera_up = glm::vec3(0.0, 1.0, 0.0);
	float fovy = 45.0f;
	float aspect = 1.0f;
	float z_near = 0.5f;
	float z_far = 20.0f;
	CameraControl& camera_control = *(new TurnTableCameraControl());

	Camera camera = Camera(camera_control, 
		                   camera_eye, 
		                   camera_center, 
		                   camera_up, 
		                   fovy, 
		                   aspect, 
		                   z_near, 
		                   z_far);

	nTiled_gui::GuiManager gui_manager = nTiled_gui::GuiManager(camera);

	// Set up GUI manager
	gui_manager.init(*window);
	// Set up Pipeline manager

	// Pipeline
	nTiled_pipeline::Pipeline& pipeline = nTiled_pipeline::ForwardPipeline(camera, 
		"./src/pipeline/shaders/basicForwardColour.vert",
		"./src/pipeline/shaders/basicForwardColour.frag");
	// World	
	nTiled_world::World world = nTiled_world::World();
	world.objectsFromOBJ(std::string("./res/cubes.obj"));

	nTiled_pipeline::PipelineManager pipeline_manager =
		nTiled_pipeline::PipelineManager(pipeline, world);
	
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

	// remove camera control from dynamic memory
	delete &camera_control;

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