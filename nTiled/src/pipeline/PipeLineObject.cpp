#include "pipeline\PipelineObject.h"

namespace nTiled {
namespace pipeline {

// ----------------------------------------------------------------------------
//  PipelineObject
// ----------------------------------------------------------------------------
PipelineObject::PipelineObject(GLuint vao,
                               GLuint element_buffer,
                               GLuint element_buffer_size,
                               glm::mat4 transformation) :
  vao(vao),
  element_buffer(element_buffer),
  element_buffer_size(element_buffer_size),
  transformation(transformation) {
}

} // pipeline
} // nTiled