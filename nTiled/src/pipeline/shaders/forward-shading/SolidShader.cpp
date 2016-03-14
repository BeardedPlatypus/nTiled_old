#include "pipeline\shaders\forward-shading\ForwardShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shader-util\LoadShaders.h"
#include "camera\Camera.h"

namespace nTiled {
namespace pipeline {

SolidShader::SolidShader(ForwardShaderId shader_id,
                         const std::string& path_vertex_shader,
                         const std::string& path_fragment_shader) :
  ForwardShader(shader_id,
                path_vertex_shader,
                path_fragment_shader) {
}

void SolidShader::loadShaders() {
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


void SolidShader::init(const camera::Camera& camera) {
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

void SolidShader::render(const camera::Camera& camera) {
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
    // unbind vao
  }
  glBindVertexArray(0);
  glUseProgram(0);
}

} // pipeline
} // nTiled