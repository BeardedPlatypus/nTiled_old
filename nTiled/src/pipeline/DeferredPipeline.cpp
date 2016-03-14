#include "pipeline\Pipeline.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

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
DeferredPipeline::DeferredPipeline(state::State& state) :
  Pipeline(state) {
  ShaderFactory shader_factory = ShaderFactory(state);

  // FIXME: currently assume that the shader to be used is the shader 
  // of the first object, this needs to be more dynamic.
  // DIRTY CODE IS DIRTY
  // *******************************************************************************
  DeferredShaderId shader_id = this->state.world.objects[0].shader_key.deferred_id;
  // *******************************************************************************

  // setup shader
  this->deferred_shader = shader_factory.getDeferredShader(shader_id);

  // add objects to shader.
  for (const world::Object& object : this->state.world.objects) {
    // only add objects that use this specific shader, ignore others
    if (object.shader_key.deferred_id == shader_id) {
      this->addObject(object);
    }
  }

  // setup lights
  for (const world::PointLight& pointLight : this->state.world.lights) {
    this->deferred_shader->addLight(&pointLight);
  }

  // initialise shader
  this->deferred_shader->init();
}

// ----------------------------------------------------------------------------
//  Render functions
void DeferredPipeline::render() {
  this->deferred_shader->render();
}

// ----------------------------------------------------------------------------
//  Object loading
void DeferredPipeline::addObject(const world::Object& object) {
  this->deferred_shader->addObject(object);
}

} // pipeline
} // nTiled