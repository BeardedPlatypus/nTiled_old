#include "pipeline\Pipeline.h"

#include <glm/gtc/type_ptr.hpp>
#include "pipeline\shader-util\LoadShaders.h"

using namespace nTiled_pipeline;

// ---------------------------------------------------------------------------
//  Forward Pipeline
// ---------------------------------------------------------------------------
//  Constructor
ForwardPipeline::ForwardPipeline(Camera& camera,
	                             const std::string vertex_shader_path,
	                             const std::string fragment_shader_path) :
	camera(&camera),
	vertex_shader_path(vertex_shader_path),
	fragment_shader_path(fragment_shader_path) {}

// ----------------------------------------------------------------------------
//  Render functions
void ForwardPipeline::init() {
	//  Set up shaders.
	// ------------------------------------------------------------------------
	this->loadShaderProgram(this->vertex_shader_path,
		                    this->fragment_shader_path);

	glm::mat4 lookAt = this->camera->getLookAt();
	glm::mat4 perspective_matrix = this->camera->getPerspectiveMatrix();
	glm::mat4 model_matrix(1.0f);

	// Load values into shader
	// ------------------------------------------------------------------------
	GLint p_cameraToClip = glGetUniformLocation(this->shader_program,
		                                        "cameraToClipMatrix");
	GLint p_worldToCamera = glGetUniformLocation(this->shader_program,
		                                         "worldToCameraMatrix");
	GLint p_modelToWorld = glGetUniformLocation(this->shader_program,
		                                        "modelToWorldMatrix");

	glUseProgram(this->shader_program);
	glUniformMatrix4fv(p_cameraToClip, 1, GL_FALSE, glm::value_ptr(perspective_matrix));
	glUniformMatrix4fv(p_worldToCamera, 1, GL_FALSE, glm::value_ptr(lookAt));
	glUniformMatrix4fv(p_modelToWorld, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUseProgram(0);
}

void ForwardPipeline::render() {
	glUseProgram(this->shader_program);
	GLint p_worldToCamera = glGetUniformLocation(this->shader_program,
		                                         "worldToCameraMatrix");
	glm::mat4 lookAt = this->camera->getLookAt();
	glUniformMatrix4fv(p_worldToCamera, 1, GL_FALSE, glm::value_ptr(lookAt));
}

// ----------------------------------------------------------------------------
//  Property functions
Camera ForwardPipeline::getActiveCamera() {
	return *(this->camera);
}
void ForwardPipeline::setActiveCamera(Camera& camera) {
	this->camera = &camera;
}

void ForwardPipeline::loadShaderProgram(const std::string vertex_shader_path,
	                                    const std::string fragment_shader_path) {
	this->shader_program = createVertexFragmentProgram(vertex_shader_path,
		                                               fragment_shader_path);
}