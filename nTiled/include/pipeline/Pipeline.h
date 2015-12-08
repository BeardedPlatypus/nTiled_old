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
	//  Pipeline classes
	class Pipeline {
	public:
		virtual ~Pipeline() {}
		// Render Methods
		virtual void init(nTiled_world::World& world) = 0;
		virtual void render() = 0;

		// Property Methods
		virtual Camera getActiveCamera() = 0;
		virtual void setActiveCamera(Camera& camera) = 0;

		virtual void addObject(nTiled_world::Object& object) = 0;
	};

	class ForwardPipeline : public Pipeline {
	public:
		ForwardPipeline(Camera& camera, 
			            std::vector<ShaderBatch*> shaders);

		// Render Methods
		void init(nTiled_world::World& world);
		void render();

		// Property Methods
		Camera getActiveCamera();
		void setActiveCamera(Camera& camera);

		void addObject(nTiled_world::Object& object);

	private:
		Camera* camera;
		
		// Render methods.
		std::vector<PipelineObject> objects;
		std::map<std::string, ShaderBatch*> shaders;
	};
}
