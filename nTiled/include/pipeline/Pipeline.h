#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <string>
#include <vector>
#include <map>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\World.h"
#include "state\State.h"

#include "pipeline\PipelineObject.h"
#include "shaders\forward-shading\ForwardShader.h"
#include "shaders\deferred-shading\DeferredShader.h"

// FIXME TEST REMOVE
#include "pipeline\debug-display\ConstantQuadWriter.h"
#include "pipeline\debug-display\LightProjectorQuadWriter.h"
#include "pipeline\debug-display\LightTilesDisplay.h"

namespace nTiled {
namespace pipeline {

// ---------------------------------------------------------------------------
//  Pipeline Base class
// ---------------------------------------------------------------------------
class Pipeline {
 public:
  Pipeline(state::State& state);
  virtual ~Pipeline() {}
  // Render Methods
  virtual void render() = 0;

  // Property Methods
  virtual void addObject(const world::Object& object) = 0;

 protected:
  state::State& state;
};

// ----------------------------------------------------------------------------
//  Forward Pipeline
// ----------------------------------------------------------------------------
class ForwardPipeline : public Pipeline {
 public:
  ForwardPipeline(state::State& state);

  // Render Methods
  void render();

  // Property Methods
  void addObject(const world::Object& object);

private:
  // Render methods.
  std::vector<PipelineObject> objects;
  std::map<ForwardShaderId, ForwardShader*> shaders;
};

// ----------------------------------------------------------------------------
//  Forward Debug Pipeline
// ----------------------------------------------------------------------------
class ForwardDebugPipeline : public Pipeline {
public:
  ForwardDebugPipeline(state::State& state);

  // Render Methods
  void render();

  // Property Methods
  void addObject(const world::Object& object);

private:
  // Render methods.
  std::vector<PipelineObject> objects;
  std::map<ForwardShaderId, ForwardShader*> shaders;

  // Additional Frame buffer objects 
  GLuint p_fbo;
  GLuint p_pass_result;

  PipelineObject fullscreen_quad;
  GLuint result_pass_sp;

  // FIXME TEST REMOVE
  ConstantQuadWriter quad_writer;
  LightProjectorQuadWriter draw_pretty_squares;

  BoxProjector projector;
  TiledLightManager manager;
  LightTilesDisplay tiles_display;
};

// ----------------------------------------------------------------------------
//  Deferred Pipeline
// ----------------------------------------------------------------------------
class DeferredPipeline : public Pipeline {
public:
  DeferredPipeline(state::State& state);

  // Render Methods
  void render();

  // Property Methods
  void addObject(const world::Object& object);

private:
  // Render methods.
  std::vector<PipelineObject> objects;
  DeferredShader* deferred_shader;
};

} // pipeline
} // nTiled
