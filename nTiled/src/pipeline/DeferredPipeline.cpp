#include "pipeline\Pipeline.h"

#include <glm/gtc/type_ptr.hpp>
#include "pipeline\PipelineObject.h"
#include "pipeline\shader-util\LoadShaders.h"

#include "pipeline\shaders\ShaderFactory.h"

#include <iostream>

using namespace nTiled_pipeline;

// ---------------------------------------------------------------------------
//  Forward Pipeline
// ---------------------------------------------------------------------------
//  Constructor
DeferredPipeline::DeferredPipeline(nTiled_state::State& state,
	                               ShaderId shader_id) :
	                               Pipeline(state) {
	ShaderFactory shader_factory = ShaderFactory(state);
	this->deferredShader = shader_factory.getShader(shader_id, state);


	for (nTiled_world::Object object : this->state.world.objects) {
		if (object.shader_id == shader_id) {
			this->addObject(object);
		}
	}

	// setup lights
	for (nTiled_world::PointLight& pointLight : this->state.world.lights) {
		this->deferredShader->addLight(&pointLight);
	}

	// initialise shader
	this->deferredShader->init(this->state.camera);
}

// ----------------------------------------------------------------------------
//  Render functions
void DeferredPipeline::render() {
	this->deferredShader->render(this->state.camera);
}

// ----------------------------------------------------------------------------
//  Object loading
void DeferredPipeline::addObject(nTiled_world::Object& object) {
	this->deferredShader->addObject(object);
}
