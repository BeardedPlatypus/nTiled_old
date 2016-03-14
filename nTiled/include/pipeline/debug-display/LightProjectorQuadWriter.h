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
#include "state\State.h"
#include "pipeline\light-management\Tiled\LightProjector.h"


namespace nTiled {
namespace pipeline {

class LightProjectorQuadWriter : public DebugShader {
public:
  LightProjectorQuadWriter(const state::State& state);
  void render();

private:
  PipelineObject quad;
  GLuint position_buffer;

  GLuint shader_program;

  const state::State& state;
  const BoxProjector projector;
};

} // pipeline
} // nTiled
