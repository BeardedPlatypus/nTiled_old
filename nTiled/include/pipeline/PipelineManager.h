#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "state\State.h"
#include "pipeline\Pipeline.h"

namespace nTiled {
namespace pipeline {

class PipelineManager {
 public:
  PipelineManager(state::State& state);
  ~PipelineManager();

  void init();
  void render();

 private:
  Pipeline* pipeline;
  state::State& state;
};

} // pipeline
} // nTiled