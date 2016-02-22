#include "pipeline\shaders\ShaderFactory.h"
#include "pipeline\shaders\forward-shading\BasicForwardLightShader.h"
#include "pipeline\shaders\forward-shading\debug\DebugLightShader.h"

#include <iostream>

using namespace nTiled_pipeline;

// ----------------------------------------------------------------------------
//  Constructor || Destructor
// ----------------------------------------------------------------------------
ShaderFactory::ShaderFactory() : shader_catalog(std::map<ShaderId, ShaderBatch*>()) {}

ShaderFactory::~ShaderFactory() {
	for (const auto& shader_pair : this->shader_catalog) {
		ShaderBatch* s = shader_pair.second;
		delete s;
	}
}

// ----------------------------------------------------------------------------
//  getShader
// ----------------------------------------------------------------------------
ShaderBatch* ShaderFactory::getShader(ShaderId shader_id) {
	std::map<ShaderId, ShaderBatch*>::iterator it = 
		this->shader_catalog.find(shader_id);

	// shader is already constructed and thus returned
	if (it != this->shader_catalog.end()) {
		return it->second;
	} // shader is not yet constructed, and thus added
	else {
		ShaderBatch* shader_p = addShader(shader_id);
		return shader_p;
	}
}


// ----------------------------------------------------------------------------
//  addShader
// ----------------------------------------------------------------------------
ShaderBatch* ShaderFactory::addShader(ShaderId shader_id) {
	ShaderBatch* p;

	// Regular Forward Shaders
	// ------------------------------------------------------------------------
	if (shader_id == ShaderId::ForwardSolid) {
		// Shader locations
		std::string path_vert_shader =
			"./src/pipeline/shaders-glsl/forward-shading/solid/solid.vert";
		std::string path_frag_shader =
			"./src/pipeline/shaders-glsl/forward-shading/solid/solid.frag";

		// Allocate new shader batch on the stack
		p = new SolidShader(ShaderId::ForwardSolid,
			                path_vert_shader,
			                path_frag_shader);
	}
	else if (shader_id == ShaderId::ForwardBasicVertex)	{
		// Shader locations
		std::string path_vert_shader =
			"./src/pipeline/shaders-glsl/forward-shading/basic/basicForwardMultipleLights.vert";
		std::string path_frag_shader =
			"./src/pipeline/shaders-glsl/forward-shading/basic/basicForwardMultipleLights.frag";

		// Allocate new shader batch on the stack
		p = new BasicForwardVertLightShader(
			ShaderId::ForwardBasicVertex,
			path_vert_shader,
			path_frag_shader);
	}
	else if (shader_id == ShaderId::ForwardBasicFrag) {
		// Shader locations
		std::string path_vert_shader =
			"./src/pipeline/shaders-glsl/forward-shading/basic/basicForwardMultipleLightsFrag.vert";
		std::string path_frag_shader =
			"./src/pipeline/shaders-glsl/forward-shading/basic/basicForwardMultipleLightsFrag.frag";

		// Allocate new shader batch on the stack
		p = new BasicForwardFragLightShader(
			ShaderId::ForwardBasicFrag,
			path_vert_shader,
			path_frag_shader);
	}
	else if (shader_id == ShaderId::ForwardAttenuated) {
		// Shader locations
		std::string path_vert_shader =
			"./src/pipeline/shaders-glsl/forward-shading/forward-attenuated/basicForwardMultipleLightsFrag.vert";
		std::string path_frag_shader =
			"./src/pipeline/shaders-glsl/forward-shading/forward-attenuated/basicForwardMultipleLightsFrag.frag";

		// Allocate new shader batch on the stack
		p = new BasicForwardFragLightShader(
			ShaderId::ForwardAttenuated,
			path_vert_shader,
			path_frag_shader);
	}
	// Tiled Forward Shaders
	// ------------------------------------------------------------------------

	// Clustered Forward Shaders
	// ------------------------------------------------------------------------

	//  Debug Forward Shaders
	// ------------------------------------------------------------------------
	else if (shader_id == ShaderId::ForwardDebugCoreLight) {
		// Shader locations
		std::string path_vert_shader =
			"./src/pipeline/shaders-glsl/forward-shading/debug/core/core.vert";
		std::string path_frag_shader =
			"./src/pipeline/shaders-glsl/forward-shading/debug/core/core.frag";

		// Allocate new shader batch on the stack
		p = new ForwardDebugCoreLight(
			ShaderId::ForwardDebugCoreLight,
			path_vert_shader,
			path_frag_shader);
	}
	else if (shader_id == ShaderId::ForwardDebugCutOffLight) {
		// Shader locations
		std::string path_vert_shader =
			"./src/pipeline/shaders-glsl/forward-shading/debug/cutoff/cutoff.vert";
		std::string path_frag_shader =
			"./src/pipeline/shaders-glsl/forward-shading/debug/cutoff/cutoff.frag";

		// Allocate new shader batch on the stack
		p = new ForwardDebugCutOffLight(
			ShaderId::ForwardDebugCutOffLight,
			path_vert_shader,
			path_frag_shader);
	}

	// Add shader to the catalog and return
	this->shader_catalog.insert(std::pair<ShaderId, ShaderBatch*>(shader_id, p));
	return p;
}

