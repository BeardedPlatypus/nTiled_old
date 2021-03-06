#include "pipeline\PipelineManager.h"

namespace nTiled {
namespace pipeline {

// ----------------------------------------------------------------------------
//  Constructor
// ----------------------------------------------------------------------------
PipelineManager::PipelineManager(state::State& state) :
  state(state) {
  /*
  switch (state.pipeline_type) {
  case PipelineType::Forward:
  {
#ifdef DEBUG
    std::cout << "Forward Pipeline constructed" << std::endl;
#endif // DEBUG

    this->pipeline = new ForwardPipeline(state);
  }
  case PipelineType::Deferred:
  {
    // FIXME: this is broken do not use DeferredShading right now
    this->pipeline = new DeferredPipeline(state,
                                        ShaderId::ForwardAttenuated);
  }
  }
  */
  if (this->state.shading.pipeline_type == PipelineType::Forward) {
    this->pipeline = new ForwardPipeline(state);
  } else if (this->state.shading.pipeline_type == PipelineType::ForwardDebug) {
    this->pipeline = new ForwardDebugPipeline(state);
  } else if (this->state.shading.pipeline_type == PipelineType::Deferred) {
    this->pipeline = new DeferredPipeline(state);
  }
  //this->pipeline = new ForwardPipeline(state);
}

PipelineManager::~PipelineManager() {
  delete this->pipeline;
}

// ----------------------------------------------------------------------------
//  Management Functions
// ----------------------------------------------------------------------------
void PipelineManager::init() {
  // Set up face culling
  // ---------------------------------------------------------------------------
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  // Set up depth buffer
  // ---------------------------------------------------------------------------
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);
}

void PipelineManager::render() {
  this->pipeline->render();
}

} // pipeline
} // nTiled