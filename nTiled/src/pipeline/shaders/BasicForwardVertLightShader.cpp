#include "pipeline\shaders\Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include "pipeline\shader-util\LoadShaders.h"
#include "camera\Camera.h"

using namespace nTiled_pipeline;

BasicForwardVertLightShader::BasicForwardVertLightShader(
	std::string name,
	std::string path_vertex_shader,
	std::string path_fragment_shader)
	: name(name),
	  obj_ps(std::vector<PipelineObject*>()),
	  path_vertex_shader(path_vertex_shader),
	  path_fragment_shader(path_fragment_shader) {
	
}

std::string BasicForwardVertLightShader::getName() {
	return this->name;
}

void BasicForwardVertLightShader::addObject(PipelineObject* obj_p) {
	this->obj_ps.push_back(obj_p);
}

void BasicForwardVertLightShader::setLight(nTiled_world::PointLight* light) {
	this->light = light;
	this->light_data = { light->position,
						 light->intensity,
						 light->cutoff_radius,
						 light->is_emitting };
}

void BasicForwardVertLightShader::init(Camera& camera) {
	// Create shader program
	// ------------------------------------------------------------------------
	this->shader_program = createVertexFragmentProgram(
		this->path_vertex_shader, 
		this->path_fragment_shader);

	// TODO: add function to change perspective matrix.
	// Setup perspective matrix
	// ------------------------------------------------------------------------
	glm::mat4 perspective_matrix = camera.getPerspectiveMatrix();
	GLint p_cameraToClip = glGetUniformLocation(this->shader_program,
		                                        "cameraToClipMatrix");

	glUseProgram(this->shader_program);
	glUniformMatrix4fv(p_cameraToClip, 
		               1, GL_FALSE, 
		               glm::value_ptr(perspective_matrix));
	glUseProgram(0);

	// Setup unchangable light aspects
	// ------------------------------------------------------------------------
	// generate UBO
	glGenBuffers(1, &this->light_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 
		         this->light_ubo);
	glBufferData(GL_UNIFORM_BUFFER, 
		         sizeof((this->light_data)),
		         &(this->light_data), 
		         GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	GLint p_lightBlock = glGetUniformBlockIndex(this->shader_program,
		                                        "LightBlock");
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->light_ubo);
	glUniformBlockBinding(this->shader_program, 
		                  p_lightBlock, 
		                  0);
}

void BasicForwardVertLightShader::render(Camera& camera) {
	// Set shader program to this
	glUseProgram(this->shader_program);

	// Update light data
	this->light_data.positionCameraCoordinates = 
		camera.getLookAt() * this->light->position;

	// copy light data to buffer
	glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);
	GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(p, &(this->light_data), sizeof(&this->light_data));
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	// render each model
	glm::mat4 lookAt = camera.getLookAt();
	for (PipelineObject* obj_p : this->obj_ps) {
		glBindVertexArray(obj_p->vao);

		// retrieve memory locations shared per model
		GLint p_modelToCamera = glGetUniformLocation(
			this->shader_program,
			"modelToCameraMatrix");
		GLint p_normalModelToCamera = glGetUniformLocation(
			this->shader_program,
			"normalModelToCameraMatrix");
		
		// calculate transformation matrices per model
		glm::mat4 modelToCamera = lookAt * obj_p->transformation;

		glm::mat3 normalModelToCamera =
			glm::transpose(glm::inverse(glm::mat3(modelToCamera)));

		glUniformMatrix4fv(p_modelToCamera, 1, GL_FALSE,
			glm::value_ptr(modelToCamera));
		glUniformMatrix3fv(p_normalModelToCamera, 1, GL_FALSE,
			glm::value_ptr(normalModelToCamera));

		// render the elements.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 
			         obj_p->element_buffer);
		glDrawElements(GL_TRIANGLES,
			           obj_p->element_buffer_size,
			           GL_UNSIGNED_INT, 0);
	}
	glUseProgram(0);
}

