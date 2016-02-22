#pragma once

#include "pipeline\shaders\forward-shading\BasicForwardLightShader.h"

namespace nTiled_pipeline {
	class ForwardDebugLight : public BasicForwardFragLightShader {
	public:
		ForwardDebugLight(ShaderId shader_id,
			              std::string vertex_shader_path,
			              std::string fragment_shader_path);
		void loadShaders();
	};

	// ------------------------------------------------------------------------
	class ForwardDebugCoreLight : public ForwardDebugLight {
	public:
		ForwardDebugCoreLight(ShaderId shader_id,
			                  std::string vertex_shader_path,
			                  std::string fragment_shader_path);
		void init(Camera& camera);
		void render(Camera& camera);
	};

	// ------------------------------------------------------------------------
	class ForwardDebugCutOffLight : public ForwardDebugLight {
	public:
		ForwardDebugCutOffLight(ShaderId shader_id,
			                    std::string vertex_shader_path,
			                    std::string fragment_shader_path);
		void init(Camera& camera);
		void render(Camera& camera);
	};
}

