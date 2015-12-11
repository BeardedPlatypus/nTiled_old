#include "pipeline\shaders\BasicDeferredLightShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include "pipeline\shader-util\LoadShaders.h"
#include "camera\Camera.h"

const GLuint WIDTH = 800, HEIGHT = 800;

using namespace nTiled_pipeline;

// ----------------------------------------------------------------------------
//  BasicDeferredLightShader class
// ----------------------------------------------------------------------------
//  Constructor
BasicDeferredLightShader::BasicDeferredLightShader(
	std::string name,
	std::string path_geometry_pass_vertex_shader,
	std::string path_geometry_pass_fragment_shader,
	std::string path_light_pass_vertex_shader,
	std::string path_light_pass_fragment_shader,
	unsigned int width, unsigned int height )
	: name(name),
	  obj_ps(std::vector<PipelineObject*>()),
	  path_geometry_pass_vertex_shader(path_geometry_pass_vertex_shader),
	  path_geometry_pass_fragment_shader(path_geometry_pass_fragment_shader),
  	  path_light_pass_vertex_shader(path_light_pass_vertex_shader),
	  path_light_pass_fragment_shader(path_light_pass_fragment_shader),
	  gBuffer(GBuffer(width, height)) {}

// ----------------------------------------------------------------------------
//  Property interaction methods
std::string BasicDeferredLightShader::getName() {
	return this->name;
}

void BasicDeferredLightShader::addObject(PipelineObject* obj_p) {
	this->obj_ps.push_back(obj_p);
}

void BasicDeferredLightShader::addLight(nTiled_world::PointLight* light) {
	this->lights.push_back(light);

	PipelineLightData data = { light->position,
		                       light->intensity,
		                       light->cutoff_radius,
		                       light->is_emitting };
	this->light_data.push_back(data);
}

// ----------------------------------------------------------------------------
//  Render related methods
void BasicDeferredLightShader::init(Camera& camera) {
	// Create shader program
	this->loadShaders();

	// Create Frame Buffer Objects (FBO)
	this->gBuffer.init();

	// Setup for the geometry pass.
	// Setup perspective matrix
	glm::mat4 perspective_matrix = camera.getPerspectiveMatrix();
	GLint p_cameraToClip = 
		glGetUniformLocation(this->geometry_pass_sp,
		                     "cameraToClipMatrix");

	glUseProgram(this->geometry_pass_sp);
	glUniformMatrix4fv(p_cameraToClip,
		               1, GL_FALSE,
		               glm::value_ptr(perspective_matrix));
	glUseProgram(0);
}

void BasicDeferredLightShader::render(Camera& camera) {
	this->renderGeometryPass(camera);
	this->renderLightPass();
}

void BasicDeferredLightShader::renderGeometryPass(Camera& camera) {
	// Setup GBuffer
	// ---------------------------------------------------------------------------
	// enable writing to gBuffer
	this->gBuffer.bindForWriting();

	// clear old contents gBuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Rendering to GBuffer
	// ---------------------------------------------------------------------------
	// Set shader program to geometry pass
	glUseProgram(this->geometry_pass_sp);

	glm::mat4 lookAt = camera.getLookAt();
	for (PipelineObject* obj_p : this->obj_ps) {
		glBindVertexArray(obj_p->vao);

		// retrieve memory locations shared per model
		GLint p_modelToCamera = glGetUniformLocation(this->geometry_pass_sp,
			                                         "modelToCameraMatrix");
		glm::mat4 modelToCamera = lookAt * obj_p->transformation;
		glUniformMatrix4fv(p_modelToCamera, 1, GL_FALSE,
			               glm::value_ptr(modelToCamera));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			         obj_p->element_buffer);
		glDrawElements(GL_TRIANGLES,
			           obj_p->element_buffer_size,
			           GL_UNSIGNED_INT, 0);
	}
	glUseProgram(0);
}

void BasicDeferredLightShader::renderLightPass() {
	// TODO
	// Restore default frame buffer and clear it.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup GBuffer
	this->gBuffer.bindForReading();

	// FIXME eeeks hard coded constants
	GLsizei HalfWidth = (GLsizei)(WIDTH / 2.0f);
	GLsizei HalfHeight = (GLsizei)(HEIGHT / 2.0f);

	this->gBuffer.setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT,
		0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	this->gBuffer.setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT,
		0, HalfHeight, HalfWidth, HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void BasicDeferredLightShader::loadShaders() {
	// Geometry Pass
	std::stringstream geometryPassVertexShaderBuffer =
		readShader(this->path_geometry_pass_vertex_shader);
	GLuint geometryPassVertexShader = compileShader(GL_VERTEX_SHADER,
		geometryPassVertexShaderBuffer.str());

	std::stringstream geometryPassFragmentShaderBuffer =
		readShader(this->path_geometry_pass_fragment_shader);
	GLuint geometryPassFragmentShader = compileShader(GL_FRAGMENT_SHADER,
		geometryPassFragmentShaderBuffer.str());

	this->geometry_pass_sp = createProgram(geometryPassVertexShader,
		                                   geometryPassFragmentShader);

	// Light Pass
	std::stringstream lightPassVertexShaderBuffer =
		readShader(this->path_light_pass_vertex_shader);
	GLuint lightPassVertexShader = compileShader(GL_VERTEX_SHADER,
		lightPassVertexShaderBuffer.str());

	std::stringstream lightPassFragmentShaderBuffer =
		readShader(this->path_light_pass_fragment_shader);
	GLuint lightPassFragmentShader = compileShader(GL_FRAGMENT_SHADER,
		lightPassFragmentShaderBuffer.str());

	this->light_pass_sp = createProgram(lightPassVertexShader, 
		                                lightPassFragmentShader);
}