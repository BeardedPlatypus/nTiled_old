#pragma once

#include "pipeline\shaders\ShaderDefinitions.h"
#include "state\State.h"
#include "pipeline\shaders\Shader.h"

#include <map>

namespace nTiled_pipeline {
	class ShaderFactory {
	public:
		// constructor 
		ShaderFactory(nTiled_state::State& state);
		ShaderFactory(std::map<ShaderId, ShaderBatch*>& shader_catalog);

		// destructor
		~ShaderFactory();

		ShaderBatch* getShader(ShaderId shader_id, nTiled_state::State& state);
	private:
		/*! Shader catalog containing all added shaders */
		std::map<ShaderId, ShaderBatch*>& shader_catalog;

		ShaderBatch* addShader(ShaderId shader_id, nTiled_state::State& state);
	};
}
