#pragma once

#include <vector>
#include <string>
#include <map>

#include "camera\Camera.h"
#include "world\World.h"
#include "world\Lights.h"
#include "world\LightConstructor.h"

#include "pipeline\PipelineType.h"

#include "pipeline\shaders\Shader.h"
#include "pipeline\shaders\ShaderDefinitions.h"


namespace nTiled_state {
	class State {
	public:
		// Constructor || Destructor
		// ------------------------------------------------------------------------
		State(std::string path);
		~State();
		// state elements of nTiled
		// ------------------------------------------------------------------------
		// Active Camera
		Camera camera;
		CameraControl* camera_control;

		// Viewport
		glm::uvec2 viewport;

		// World
		nTiled_world::World world;
		// Shaders
		std::map<nTiled_pipeline::ShaderId, nTiled_pipeline::ShaderBatch*> shader_catalog;
		// Pipeline type
		nTiled_pipeline::PipelineType pipeline_type;
	};


	void parseLights(std::string path, 
  	                 nTiled_world::LightConstructor& constructor);
	void parseGeometry(std::string path,
		               nTiled_world::World& world);
}

