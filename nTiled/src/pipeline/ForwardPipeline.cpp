#include "pipeline\Pipeline.h"

#include <glm/gtc/type_ptr.hpp>
#include "pipeline\PipelineObject.h"
#include "pipeline\shader-util\LoadShaders.h"

#include "pipeline\shaders\ShaderFactory.h"

#include <set>
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
	// FIXME make this nice and incorporate with gui
	//std::set<ShaderId> disabled_shaders = {};
	std::set<ShaderId> disabled_shaders = { ShaderId::ForwardDebugCoreLight,
				  						    ShaderId::ForwardDebugCutOffLight
	                                        };

	for (const auto& shader_couple : this->shaders) {
		ShaderBatch* b = shader_couple.second;
		if (disabled_shaders.count(b->getId()) == 0) {
			b->render(this->state.camera);
		}
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
		ShaderFactory shader_factory = ShaderFactory(this->state);
		this->shaders.insert(std::pair<ShaderId, ShaderBatch*>(
			object.shader_id,
			shader_factory.getShader(object.shader_id, this->state)));
	}

	// Add object to shader
	// -------------------------------------------------------------------------
	this->shaders[object.shader_id]->addObject(object);
}