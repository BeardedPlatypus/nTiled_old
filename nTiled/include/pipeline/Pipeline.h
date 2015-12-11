#pragma once

#include <glad\glad.h>
#include <string>
#include <vector>
#include <map>

#include "camera\Camera.h"
#include "world\World.h"

#include "pipeline\PipelineObject.h"
#include "pipeline\shaders\Shader.h"

namespace nTiled_pipeline {
	// ---------------------------------------------------------------------------
	//  Pipeline Base class
	// ---------------------------------------------------------------------------
	class Pipeline {
	public:
		Pipeline(Camera& camera);
		virtual ~Pipeline() {}
		// Render Methods
		virtual void init(nTiled_world::World& world) = 0;
		virtual void render() = 0;

		// Property Methods
		Camera getActiveCamera();
		void setActiveCamera(Camera& camera);

		virtual void addObject(nTiled_world::Object& object) = 0;
	protected:
		Camera* camera;
	};

	// ----------------------------------------------------------------------------
	//  Forward Pipeline
	// ----------------------------------------------------------------------------
	class ForwardPipeline : public Pipeline {
	public:
		ForwardPipeline(Camera& camera, 
			            std::vector<ShaderBatch*> shaders);

		// Render Methods
		void init(nTiled_world::World& world);
		void render();

		// Property Methods
		void addObject(nTiled_world::Object& object);

	private:		
		// Render methods.
		std::vector<PipelineObject> objects;
		std::map<std::string, ShaderBatch*> shaders;
	};

	// ----------------------------------------------------------------------------
	//  Deferred Pipeline
	// ----------------------------------------------------------------------------
	class DeferredPipeline : public Pipeline {
	public:
		DeferredPipeline(Camera& camera,
			             ShaderBatch& shader);

		// Render Methods
		void init(nTiled_world::World& world);
		void render();

		// Property Methods
		void addObject(nTiled_world::Object& object);
	
	private:
		// Render methods.
		std::vector<PipelineObject> objects;
		ShaderBatch* deferredShader;
	};
}
