#include "pipeline\shaders\forward-shading\debug\DebugLightShader.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

#include "pipeline\shader-util\LoadShaders.h"
#include "camera\Camera.h"
using namespace nTiled_pipeline;

ForwardDebugLight::ForwardDebugLight(ShaderId shader_id,
 	                                 std::string vertex_shader_path,
	                                 std::string fragment_shader_path) : 
	BasicForwardFragLightShader(shader_id,
		                        vertex_shader_path,
		                        fragment_shader_path) {}

void ForwardDebugLight::loadShaders() {
	// Vertex Shader
	// -----------------------------------------------------------------
	std::stringstream vertexShaderBuffer =
		readShader(this->path_vertex_shader);
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER,
		vertexShaderBuffer.str());

	// Fragment Shader
	// -----------------------------------------------------------------
	std::stringstream fragmentShaderBuffer = 
		readShader(this->path_fragment_shader);

	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER,
		                                  fragmentShaderBuffer.str());
	this->shader_program = createProgram(vertexShader, fragmentShader);


}
// ----------------------------------------------------------------------------
//  ForwardDebugCore shader
// ----------------------------------------------------------------------------
ForwardDebugCoreLight::ForwardDebugCoreLight(ShaderId shader_id,
 	                                     std::string vertex_shader_path,
	                                     std::string fragment_shader_path) : 
	ForwardDebugLight(shader_id,
		              vertex_shader_path,
		              fragment_shader_path) {}

void ForwardDebugCoreLight::init(Camera& camera) {
	// Create shader program
	// ------------------------------------------------------------------------
	std::cout << "initing debug core light" << std::endl;

	this->loadShaders();

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
}

void ForwardDebugCoreLight::render(Camera& camera) {
	// Set shader program to this
	glUseProgram(this->shader_program);

	std::cout << "ping" << std::endl;

	// Set camera matrix
	glm::mat4 lookAt = camera.getLookAt();

	// Render models
	for (PipelineObject obj : this->objects) {
		// Bind vertex array object of this model
		glBindVertexArray(obj.vao);

		// Set camera position
		GLint p_modelToCamera = glGetUniformLocation(
			this->shader_program,
			"modelToCameraMatrix");

		glm::mat4 modelToCamera = lookAt * obj.transformation;

		glUniformMatrix4fv(p_modelToCamera, 
			               1, 
			               GL_FALSE,
			               glm::value_ptr(modelToCamera));

		// render the elements
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			obj.element_buffer);
		glDrawElements(GL_TRIANGLES,
			obj.element_buffer_size,
			GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
	glUseProgram(0);
}


// ----------------------------------------------------------------------------
//  ForwardDebugCutoff shader
// ----------------------------------------------------------------------------
ForwardDebugCutOffLight::ForwardDebugCutOffLight(ShaderId shader_id,
 	                                     std::string vertex_shader_path,
	                                     std::string fragment_shader_path) : 
	ForwardDebugLight(shader_id,
		              vertex_shader_path,
		              fragment_shader_path) {}
void ForwardDebugCutOffLight::init(Camera& camera) {
	// Create shader program
	// ------------------------------------------------------------------------
	this->loadShaders();
	std::cout << "initing debug cutoff light" << std::endl;

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
}


void ForwardDebugCutOffLight::render(Camera& camera) {
	// Set shader program to this
	glUseProgram(this->shader_program);

	// Set camera matrix
	glm::mat4 lookAt = camera.getLookAt();

	// Render models
	for (PipelineObject obj : this->objects) {
		// Bind vertex array object of this model
		glBindVertexArray(obj.vao);

		// Set camera position
		GLint p_modelToCamera = glGetUniformLocation(
			this->shader_program,
			"modelToCameraMatrix");

		glm::mat4 modelToCamera = lookAt * obj.transformation;

		glUniformMatrix4fv(p_modelToCamera, 
			               1, 
			               GL_FALSE,
			               glm::value_ptr(modelToCamera));

		// render the elements
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			obj.element_buffer);
		glDrawElements(GL_TRIANGLES,
			obj.element_buffer_size,
			GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
	glUseProgram(0);
}