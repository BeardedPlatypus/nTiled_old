#include <iostream>

#include <imgui.h>
#include "gui/imgui_impl_glfw_gl3.h"

// GLAD
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>

#include "gui\GuiManager.h"
#include "pipeline\PipelineManager.h"
#include "world\ObjectConstructors.h"
#include "pipeline\shaders\BasicForwardLightShader.h"
// debugger amd / nvidia openGL

//#define VERT_SHADER_PATH std::string("./src/pipeline/shaders-glsl/basicForwardMultipleLights.vert")
//#define FRAG_SHADER_PATH std::string("./src/pipeline/shaders-glsl/basicForwardMultipleLights.frag")
#define VERT_SHADER_PATH std::string("./src/pipeline/shaders-glsl/basicForwardMultipleLightsFrag.vert")
#define FRAG_SHADER_PATH std::string("./src/pipeline/shaders-glsl/basicForwardMultipleLightsFrag.frag")


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

	std::cout << "loaded opengl " << glGetString(GL_VERSION) << std::endl;

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

	// Shaders
	std::string basic_shader_id = std::string("basic");
	/*
	nTiled_pipeline::BasicForwardVertLightShader shader =
		nTiled_pipeline::BasicForwardVertLightShader(basic_shader_id, 
			                                         VERT_SHADER_PATH, 
			                                         FRAG_SHADER_PATH);
    */
	nTiled_pipeline::BasicForwardFragLightShader shader =
		nTiled_pipeline::BasicForwardFragLightShader(basic_shader_id,
			                                         VERT_SHADER_PATH,
			                                         FRAG_SHADER_PATH);

	std::vector<nTiled_pipeline::ShaderBatch*> shaders = { &shader };

	// World	
	nTiled_world::World world = nTiled_world::World();

	glm::mat4 transformation =
		glm::mat4(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.0, 0.0, 0.0, 1.0
			);

	nTiled_world::ObjConstructor suzanneConstructor =
		nTiled_world::ObjConstructor(world, std::string("./res/suzanne.obj"));
	suzanneConstructor.add(std::string("suzanne"), 
		                   basic_shader_id,
		                   transformation);

	
	glm::mat4 transformation1 =
		glm::mat4(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 2.5, 0.0, 1.0
			);
	/*
	nTiled_world::IcosphereConstructor icosphereConstructor =
		nTiled_world::IcosphereConstructor(world);

	icosphereConstructor.add("sphere", 
		                     std::string("basic"),
		                     transformation1);
	*/
	// add lights real dirty
	nTiled_world::PointLight light = nTiled_world::PointLight(
		glm::vec4(0.0f, 5.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		10.0f,
		true,
		world.objects[0],
		world.objects[0]);
	world.lights.push_back(light);

	/*
	world.objectFromOBJ(std::string("./res/cube2.obj"), transformation1);
	
	glm::mat4 transformation2 = 
		glm::mat4(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.75, 0.0, 1.0
			);

	world.objectFromOBJ(std::string("./res/cube2.obj"), transformation2);
	*/

	//std::cout << world.objects[1].mesh.vertices.size() << std::endl;

	// Pipeline
	nTiled_pipeline::Pipeline& pipeline = nTiled_pipeline::ForwardPipeline(camera, shaders);

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