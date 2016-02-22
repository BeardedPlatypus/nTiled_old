#include "pipeline\Pipeline.h"

#include <glm/gtc/type_ptr.hpp>
#include "pipeline\PipelineObject.h"
#include "pipeline\shader-util\LoadShaders.h"

#include <iostream>

using namespace nTiled_pipeline;

// ---------------------------------------------------------------------------
//  Forward Pipeline
// ---------------------------------------------------------------------------
//  Constructor
ForwardPipeline::ForwardPipeline(nTiled_state::State& state) :
	Pipeline(state),
	objects(std::vector<PipelineObject>()),
	shaders(std::map<ShaderId, ShaderBatch*>()) {
	std::cout << "forward pipeline" << std::endl;
	std::cout << "number of objects: " << state.world.objects.size() << std::endl;
	
	// setup objects and add all relevant shaders
	for (nTiled_world::Object object : this->state.world.objects) {
		std::cout << "object" << std::endl;
		this->addObject(object);
	}

	// Add lights to all relevant shaders 
	for (const auto& shader_couple : this->shaders) {
		ShaderBatch* b = shader_couple.second;
		for (nTiled_world::PointLight& pointLight : this->state.world.lights) {
			b->addLight(&pointLight);
		}
	}

	// initialise each shader
	ShaderBatch* b_p;
	for (const auto& shader_couple : this->shaders) {
		b_p = shader_couple.second;
		b_p->init(this->state.camera);
	}
}

// ----------------------------------------------------------------------------
//  Render functions
void ForwardPipeline::render() {
	for (const auto& shader_couple : this->shaders) {
		ShaderBatch* b = shader_couple.second;
		b->render(this->state.camera);
	}
	//this->shaders[ShaderId::ForwardBasicFrag]->render(this->state.camera);
}

// ----------------------------------------------------------------------------
//  Object loading
void ForwardPipeline::addObject(nTiled_world::Object& object) {
	// Add shader if it has not been initialised yet.
	// ------------------------------------------------------------------------
	if (this->shaders.count(object.shader_id) == 0) {
		// FIXME Rework this together with shaderfactor
		this->shaders.insert(std::pair<ShaderId, ShaderBatch*>(
			object.shader_id,
			this->state.shader_factory.getShader(object.shader_id)));
	}

	// Add object to shader
	// -------------------------------------------------------------------------
	this->shaders[object.shader_id]->addObject(object);
}