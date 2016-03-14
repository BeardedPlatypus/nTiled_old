#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>
#include <glm\glm.hpp>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\debug-display\DebugShader.h"
#include "pipeline\PipelineObject.h"

namespace nTiled {
namespace pipeline {
class ConstantQuadWriter : public DebugShader {
public:
  ConstantQuadWriter();
  void render();

  void addQuad(glm::vec4 ndc_coordinates);

private:
  std::vector<PipelineObject> quads;

  GLuint shader_program;
};

} // pipeline
} // nTiled
