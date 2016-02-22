#pragma once

#include <glad\glad.h>
#include <string>
#include <vector>
#include <map>

#include "camera\Camera.h"
#include "world\World.h"
#include "state\State.h"

#include "pipeline\PipelineObject.h"
#include "pipeline\shaders\Shader.h"

namespace nTiled_pipeline {
	// ---------------------------------------------------------------------------
	//  Pipeline Base class
	// ---------------------------------------------------------------------------
	class Pipeline {
	public:
		Pipeline(nTiled_state::State& state);
		virtual ~Pipeline() {}
		// Render Methods
		virtual void render() = 0;

		// Property Methods
		nTiled_state::State& state;
		virtual void addObject(nTiled_world::Object& object) = 0;
	};

	// ----------------------------------------------------------------------------
	//  Forward Pipeline
	// ----------------------------------------------------------------------------
	class ForwardPipeline : public Pipeline {
	public:
		ForwardPipeline(nTiled_state::State& state);

		// Render Methods
		void render();

		// Property Methods
		void addObject(nTiled_world::Object& object);

	private:		
		// Render methods.
		std::vector<PipelineObject> objects;
		std::map<ShaderId, ShaderBatch*> shaders;
	};

	// ----------------------------------------------------------------------------
	//  Deferred Pipeline
	// ----------------------------------------------------------------------------
	class DeferredPipeline : public Pipeline {
	public:
		DeferredPipeline(nTiled_state::State& state,
			             ShaderId shader_id);

		// Render Methods
		void render();

		// Property Methods
		void addObject(nTiled_world::Object& object);
	
	private:
		// Render methods.
		std::vector<PipelineObject> objects;
		ShaderBatch* deferredShader;
	};
}
