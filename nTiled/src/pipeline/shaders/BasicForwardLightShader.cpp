#include "pipeline\shaders\BasicForwardLightShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include "pipeline\shader-util\LoadShaders.h"
#include "camera\Camera.h"

#define MAX_N_LIGHTS 20

using namespace nTiled_pipeline;

// ----------------------------------------------------------------------------
//  BasicForwardLightShader Base class
// ----------------------------------------------------------------------------
BasicForwardLightShader::BasicForwardLightShader(
	std::string name,
	std::string path_vertex_shader,
	std::string path_fragment_shader)
	: name(name),
	  obj_ps(std::vector<PipelineObject*>()),
	  path_vertex_shader(path_vertex_shader),
	  path_fragment_shader(path_fragment_shader) {
	
}

std::string BasicForwardLightShader::getName() {
	return this->name;
}

void BasicForwardLightShader::addObject(PipelineObject* obj_p) {
	this->obj_ps.push_back(obj_p);
}

void BasicForwardLightShader::addLight(nTiled_world::PointLight* light) {
	this->lights.push_back(light);
	
	PipelineLightData data = { light->position,
						       light->intensity,
						       light->cutoff_radius,
						       light->is_emitting };
	this->light_data.push_back(data);
}

void BasicForwardLightShader::init(Camera& camera) {
	// Create shader program
	// ------------------------------------------------------------------------
	this->loadShaders();

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
	if (this->light_data.size() > 0) {
		GLuint num_lights = this->light_data.size();

		glGenBuffers(1, &this->light_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER,
			this->light_ubo);

		glBufferData(GL_UNIFORM_BUFFER,
			sizeof((this->light_data[0])) * num_lights,
			this->light_data.data(),
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// link UBO with the shader
		GLint p_lightBlock = glGetUniformBlockIndex(this->shader_program,
			                                        "LightBlock");
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->light_ubo);
		glUniformBlockBinding(this->shader_program,
			                  p_lightBlock,
			                  0);

		// add initial data
		glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);
		GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		memcpy(p, 
			   this->light_data.data(), 
			   sizeof(this->light_data[0]) * num_lights);
		glUnmapBuffer(GL_UNIFORM_BUFFER);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

// ----------------------------------------------------------------------------
//  Actual subclasses
// ----------------------------------------------------------------------------
BasicForwardVertLightShader::BasicForwardVertLightShader(
	std::string name,
	std::string path_vertex_shader,
	std::string path_fragment_shader) : BasicForwardLightShader(
		name,
		path_vertex_shader,
		path_fragment_shader) {}

void BasicForwardVertLightShader::render(Camera& camera) {
	// Set shader program to this
	glUseProgram(this->shader_program);

	// Get the lookAt matrix
	glm::mat4 lookAt = camera.getLookAt();

	// Update light data
	glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);
	for (GLuint i = 0; i < this->light_data.size(); i++) {
		glm::vec4 lightCameraCoordinates =
			lookAt * this->light_data[i].positionCoordinates;
		glBufferSubData(
			GL_UNIFORM_BUFFER,
			sizeof(this->light_data[0]) * i,
			sizeof(lightCameraCoordinates),
			glm::value_ptr(lightCameraCoordinates)
			);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// render each model
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

void BasicForwardVertLightShader::loadShaders() {
	// Vertex Shader
	// -----------------------------------------------------------------
	// load file and preprocess
	std::stringstream vertexShaderBuffer;
	
	std::string replaceLine = "#define NUM_LIGHTS ";
	std::string n_lights = std::to_string(this->light_data.size());

	std::ifstream file_in(this->path_vertex_shader, std::ifstream::in);

	for (std::string line; std::getline(file_in, line);) {
		if (line.compare(0, replaceLine.size(), replaceLine) == 0) {
			vertexShaderBuffer << replaceLine << n_lights << std::endl;
		}
		else {
			vertexShaderBuffer << line << std::endl;
		}
	}

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
BasicForwardFragLightShader::BasicForwardFragLightShader(
	std::string name,
	std::string path_vertex_shader,
	std::string path_fragment_shader) : BasicForwardLightShader(
		name,
		path_vertex_shader,
		path_fragment_shader) {}

void BasicForwardFragLightShader::render(Camera& camera) {
	// Set shader program to this
	glUseProgram(this->shader_program);
	// Get the lookAt matrix
	glm::mat4 lookAt = camera.getLookAt();

	// render each model
	for (PipelineObject* obj_p : this->obj_ps) {
		glBindVertexArray(obj_p->vao);
		// retrieve memory locations shared per model
		GLint p_modelToCamera = glGetUniformLocation(
			this->shader_program,
			"modelToCameraMatrix");

		// calculate transformation matrices per model
		glm::mat4 modelToCamera = lookAt * obj_p->transformation;
		glUniformMatrix4fv(p_modelToCamera, 1, GL_FALSE,
			glm::value_ptr(modelToCamera));

		// Update light data
		glm::mat4 worldToModelSpace = glm::inverse(obj_p->transformation);

		glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);
		for (GLuint i = 0; i < this->light_data.size(); i++) {
			glm::vec4 lightModelCoordinates =
				worldToModelSpace * this->light_data[i].positionCoordinates;
			glBufferSubData(
				GL_UNIFORM_BUFFER,
				sizeof(this->light_data[0]) * i,
				sizeof(lightModelCoordinates),
				glm::value_ptr(lightModelCoordinates)
				);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// render the elements.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			obj_p->element_buffer);
		glDrawElements(GL_TRIANGLES,
			obj_p->element_buffer_size,
			GL_UNSIGNED_INT, 0);
	}
	glUseProgram(0);
}

void BasicForwardFragLightShader::loadShaders() {
	// Vertex Shader
	// -----------------------------------------------------------------
	std::stringstream vertexShaderBuffer =
		readShader(this->path_vertex_shader);
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER,
		vertexShaderBuffer.str());

	// Fragment Shader
	// -----------------------------------------------------------------
	std::stringstream fragmentShaderBuffer;

	std::string replaceLine = "#define NUM_LIGHTS ";
	std::string n_lights = std::to_string(this->light_data.size());

	std::ifstream file_in(this->path_fragment_shader, std::ifstream::in);

	for (std::string line; std::getline(file_in, line);) {
		if (line.compare(0, replaceLine.size(), replaceLine) == 0) {
			fragmentShaderBuffer << replaceLine << n_lights << std::endl;
		}
		else {
			fragmentShaderBuffer << line << std::endl;
		}
	}

	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER,
		                                  fragmentShaderBuffer.str());

	this->shader_program = createProgram(vertexShader, fragmentShader);
}
