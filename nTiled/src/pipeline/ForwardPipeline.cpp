#include "pipeline\Pipeline.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

#include <set>
#include <iostream>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\PipelineObject.h"
#include "pipeline\shader-util\LoadShaders.h"
#include "pipeline\shaders\ShaderFactory.h"
namespace nTiled {
namespace pipeline {

// ---------------------------------------------------------------------------
//  Forward Pipeline
// ---------------------------------------------------------------------------
//  Constructor
ForwardPipeline::ForwardPipeline(state::State& state) :
  Pipeline(state),
  objects(std::vector<PipelineObject>()),
  shaders(std::map<ForwardShaderId, ForwardShader*>()) {
  std::cout << "forward pipeline" << std::endl;
  std::cout << "number of objects: " << state.world.objects.size() << std::endl;

  // setup objects and add all relevant shaders
  for (const world::Object& object : this->state.world.objects) {
    std::cout << "object" << std::endl;
    this->addObject(object);
  }

  // Add lights to all relevant shaders 
  for (const auto& shader_couple : this->shaders) {
    ForwardShader* s = shader_couple.second;
    for (const world::PointLight& pointLight : this->state.world.lights) {
      s->addLight(&pointLight);
    }
  }

  // initialise each shader
  ForwardShader* p_s;
  for (const auto& shader_couple : this->shaders) {
    p_s = shader_couple.second;
    p_s->init(this->state.view.camera);
  }
}

// ----------------------------------------------------------------------------
//  Render functions
void ForwardPipeline::render() {
  // FIXME make this nice and incorporate with gui
  //std::set<ShaderId> disabled_shaders = {};
  std::set<ForwardShaderId> disabled_shaders = {
    ForwardShaderId::ForwardDebugCoreLight,
    ForwardShaderId::ForwardDebugCutOffLight };

  for (const auto& shader_couple : this->shaders) {
    ForwardShader* p_s = shader_couple.second;
    if (disabled_shaders.count(p_s->getId()) == 0) {
      p_s->render(this->state.view.camera);
    }
  }
  //this->shaders[ShaderId::ForwardBasicFrag]->render(this->state.camera);
}

// ----------------------------------------------------------------------------
//  Object loading
void ForwardPipeline::addObject(const world::Object& object) {
  // Add shader if it has not been initialised yet.
  // ------------------------------------------------------------------------
  ForwardShaderId shader_id = object.shader_key.forward_id;
  if (this->shaders.count(shader_id) == 0) {
    // FIXME Rework this together with shaderfactor
    ShaderFactory shader_factory = ShaderFactory(this->state);
    this->shaders.insert(std::pair<ForwardShaderId, ForwardShader*>(
      shader_id,
      shader_factory.getForwardShader(shader_id)));
  }

  // Add object to shader
  // -------------------------------------------------------------------------
  this->shaders[shader_id]->addObject(object);
}

}
}