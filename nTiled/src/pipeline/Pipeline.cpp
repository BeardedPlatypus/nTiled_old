#include "pipeline\Pipeline.h"

#include <glm/gtc/type_ptr.hpp>
#include "pipeline\PipelineObject.h"
#include "pipeline\shader-util\LoadShaders.h"

#include <iostream>

using namespace nTiled_pipeline;

// ---------------------------------------------------------------------------
//  Forward Pipeline
// ---------------------------------------------------------------------------
//  Constructor
ForwardPipeline::ForwardPipeline(Camera& camera,
	                             std::vector<ShaderBatch*> shaders) :
	camera(&camera),
	objects(std::vector<PipelineObject>()) {
	for (ShaderBatch* shader : shaders) {
		this->shaders[shader->getName()] = shader;
	}
}

// ----------------------------------------------------------------------------
//  Render functions
void ForwardPipeline::init(nTiled_world::World& world) {
	// setup objects
	for (nTiled_world::Object object : world.objects) {
		this->addObject(object);
	}

	// setup lights
	for (const auto& shader_couple : this->shaders) {
		ShaderBatch* b = shader_couple.second;
		b->setLight(&world.lights[0]);
	}

	// initialise each shader
	for (const auto& shader_couple : this->shaders) {
		ShaderBatch* b = shader_couple.second;
		b->init(this->getActiveCamera());
	}
}

void ForwardPipeline::render() {
	for (const auto& shader_couple : this->shaders) {
		ShaderBatch* b = shader_couple.second;
		b->render(this->getActiveCamera());
	}
}

// ----------------------------------------------------------------------------
//  Object loading
void ForwardPipeline::addObject(nTiled_world::Object& object) {
	// Setup initial Buffer Objects
	// ------------------------------------------------------------------------
	// Set up position buffer of the vertices
	GLuint vbo_handles[3]; // create 3 vertex buffer objects
	glGenBuffers(3, vbo_handles);

	GLuint position_buffer = vbo_handles[0];
	GLuint normal_buffer = vbo_handles[1];
	GLuint element_buffer = vbo_handles[2];
	GLuint vao;

	// Set up vertex array object and enable vertex array zero and one
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
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
		object.mesh.vertices.size() * sizeof(glm::vec4),
		&object.mesh.vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0,         // attribute
		4,         // size 
		GL_FLOAT,  // type 
		GL_FALSE,  // normalized? 
		0,         // stride 
		NULL       // array buffer offset
		);

	// set up normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER,
		object.mesh.normals.size() * sizeof(glm::vec3),
		&object.mesh.normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// set up element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		object.mesh.elements.size() * sizeof(glm::tvec3<glm::u32>),
		&object.mesh.elements[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_UNSIGNED_INT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	this->objects.push_back(PipelineObject(vao,
		                                   element_buffer,
		                                   object.mesh.elements.size() * 3,
		                                   object.transformationMatrix));
	this->shaders[object.shader_id]->addObject(&(this->objects[this->objects.size() - 1]));
}

// ----------------------------------------------------------------------------
//  Property functions
Camera ForwardPipeline::getActiveCamera() {
	return *(this->camera);
}

void ForwardPipeline::setActiveCamera(Camera& camera) {
	this->camera = &camera;
}