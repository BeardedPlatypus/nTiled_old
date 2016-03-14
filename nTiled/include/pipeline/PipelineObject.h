#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <glm\glm.hpp>


namespace nTiled {
namespace pipeline {

// ---------------------------------------------------------------------------
// Pipeline data structs
struct PipelineObject {
public:
  // constructor
  PipelineObject() {};
  PipelineObject(GLuint vao,
                 GLuint element_buffer,
                 GLuint element_buffer_size,
                 glm::mat4 transformation);

  // member variables
  GLuint vao;
  GLuint element_buffer;
  GLuint element_buffer_size;
  glm::mat4 transformation;
};

struct PipelineLightData {
  glm::vec4 positionCoordinates;
  glm::vec3 intensity;
  float radius;
  int is_emitting;
  int _pad[3]; // Add 3 padding to align PipeLineLightData for gpu
};

}
}

