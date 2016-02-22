#pragma once

#include "pipeline\shaders\ShaderDefinitions.h"
#include "pipeline\shaders\Shader.h"

#include <map>

namespace nTiled_pipeline {
	class ShaderFactory {
	public:
		// constructor 
		ShaderFactory();
		// destructor
		~ShaderFactory();

		ShaderBatch* getShader(ShaderId shader_id);
	private:
		/*! Shader catalog containing all added shaders */
		std::map<ShaderId, ShaderBatch*> shader_catalog;

		ShaderBatch* addShader(ShaderId shader_id);
	};
}
