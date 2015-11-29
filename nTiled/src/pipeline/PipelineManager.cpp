#include "pipeline\PipelineManager.h"

#include <glad\glad.h>

using namespace nTiled_pipeline;
using namespace nTiled_world;
// ----------------------------------------------------------------------------
//  Constructor
// ----------------------------------------------------------------------------
PipelineManager::PipelineManager(Pipeline& pipeline, World& world) : 
	world(&world),
	pipeline(&pipeline) {}

// ----------------------------------------------------------------------------
//  Management Functions
// ----------------------------------------------------------------------------
void PipelineManager::init() {
	// Setup initial Buffer Objects
	// ------------------------------------------------------------------------
	// Set up position buffer of the vertices
	GLuint vbo_handles[3]; // create 3 vertex buffer objects
	glGenBuffers(3, vbo_handles);
	
	GLuint position_buffer = vbo_handles[0];
	GLuint normal_buffer = vbo_handles[1];
	this->element_buffer = vbo_handles[2];

	// Set up 
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// TODO: this might get changed as world gets implemented
	// Set up geometric data.
	// ------------------------------------------------------------------------
	//  Load models
	std::vector<glm::vec4> vertices = (this->world->objects[0])->mesh.vertices;
	std::vector<glm::vec3> normals = (this->world->objects[0])->mesh.normals;
	std::vector<glm::tvec3<glm::uint32>> elements = 
		(this->world->objects[0])->mesh.elements;

	// Load into memory
	// position
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4),
		         &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	
	// normal
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
		         &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// element
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		         elements.size() * sizeof(glm::tvec3<glm::u32>),
		         &elements[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_UNSIGNED_INT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	// Initialise pipeline
	// ---------------------------------------------------------------------------
	this->pipeline->init();
	
	// Set up face culling
	// ---------------------------------------------------------------------------
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Set up depth buffer
	// ---------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
}

void PipelineManager::render() {
	glBindVertexArray(this->vao);
	this->pipeline->render();
}