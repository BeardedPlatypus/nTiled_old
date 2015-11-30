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
	GLuint colour_buffer = vbo_handles[1];
	this->element_buffer = vbo_handles[2];

	// Set up vertex array object and enable vertex array zero and one
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// TODO: this might get changed as world gets implemented
	// Set up geometric data.
	// ------------------------------------------------------------------------
	// set up position buffer
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer); // set position buffer to GL_ARRAY_BUFFEr
													// not part of the VAO yet.
	glBufferData(GL_ARRAY_BUFFER,
		this->world->objects[0].mesh.vertices.size() * sizeof(glm::vec4),
		&this->world->objects[0].mesh.vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	// set up colour buffer
	std::vector<GLfloat> vertex_colors = {
		// front colours
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		// back colours
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		// front colours
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		// back colours
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_colors.size() * 3,
		&vertex_colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// set up element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->world->objects[0].mesh.elements.size() * sizeof(glm::tvec3<glm::u32>),
		&this->world->objects[0].mesh.elements[0], GL_STATIC_DRAW);
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glDrawElements(GL_TRIANGLES, 24 * 3, GL_UNSIGNED_INT, 0);
}