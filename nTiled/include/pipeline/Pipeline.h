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

// FIXME TEST REMOVE
#include "pipeline\debug-display\ConstantQuadWriter.h"
#include "pipeline\debug-display\LightProjectorQuadWriter.h"
#include "pipeline\debug-display\LightTilesDisplay.h"

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

	// Let's be silly
	class ForwardDebugPipeline : public Pipeline {
	public:
		ForwardDebugPipeline(nTiled_state::State& state);

		// Render Methods
		void render();

		// Property Methods
		void addObject(nTiled_world::Object& object);

	private:
		// Render methods.
		std::vector<PipelineObject> objects;
		std::map<ShaderId, ShaderBatch*> shaders;

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
