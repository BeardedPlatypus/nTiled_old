#include "pipeline\debug-display\LightProjectorQuadWriter.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shader-util\LoadShaders.h"

#define VERT_SHADER_PATH std::string("./src/pipeline/shaders-glsl/forward-shading/debug-shaders/quad/quad.vert")
#define FRAG_SHADER_PATH std::string("./src/pipeline/shaders-glsl/forward-shading/debug-shaders/quad/quad.frag")

namespace nTiled {
namespace pipeline {

LightProjectorQuadWriter::LightProjectorQuadWriter(const state::State& state) : state(state) {
  // Vertex Shader
  // -----------------------------------------------------------------
  std::stringstream vertexShaderBuffer =
    readShader(VERT_SHADER_PATH);
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER,
                                      vertexShaderBuffer.str());

  // Fragment Shader
  // -----------------------------------------------------------------
  std::stringstream fragmentShaderBuffer =
    readShader(FRAG_SHADER_PATH);

  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER,
                                        fragmentShaderBuffer.str());
  this->shader_program = createProgram(vertexShader, fragmentShader);

  // create quad
  // ---------------------------------------------------------------------------
  // Position data
  GLfloat quad_vertices[] = {
    0.0f, 0.0f, -.8f,
    1.0f, 0.0f, -.8f,
    1.0f, 1.0f, -.8f,
    0.0f, 1.0f, -.8f
  };

  // Element data
  GLushort elements[] = {
    0, 1,
    1, 2,
    2, 3,
    3, 0
  };

  // setup buffers
  GLuint vbo_handles[2];
  glGenBuffers(2, vbo_handles);

  this->position_buffer = vbo_handles[0];
  GLuint element_buffer = vbo_handles[1];

  // setup vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  // set up position buffer
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER,
               12 * sizeof(GLfloat),
               quad_vertices,
               GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  // set up element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               8 * sizeof(GLushort),
               elements,
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_UNSIGNED_SHORT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //glBindVertexArray(0);

  this->quad = PipelineObject(vao,
                              element_buffer,
                              8,
                              glm::mat4(1.0f));
}


void LightProjectorQuadWriter::render() {
  glUseProgram(this->shader_program);
  PipelineObject obj = this->quad;
  glBindVertexArray(obj.vao);

  glm::vec4 ndc_coordinates;
  for (const world::PointLight& light : this->state.world.lights) {
    if (this->projector.computeNDCProjection(light,
                                             this->state.view.camera,
                                             ndc_coordinates)) {
      GLfloat quad_vertices[] = {
        ndc_coordinates.x, ndc_coordinates.y, -0.8f,
        ndc_coordinates.z, ndc_coordinates.y, -0.8f,
        ndc_coordinates.z, ndc_coordinates.w, -0.8f,
        ndc_coordinates.x, ndc_coordinates.w, -0.8f
      };

      glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
      glBufferSubData(GL_ARRAY_BUFFER,
                      0,
                      12 * sizeof(GLfloat),
                      quad_vertices);

      // render quad
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                   obj.element_buffer);
      glDrawElements(GL_LINES,
                     obj.element_buffer_size,
                     GL_UNSIGNED_SHORT, 0);
    }
  }

  glBindVertexArray(0);
  glUseProgram(0);
}

} // pipeline
} // nTiled