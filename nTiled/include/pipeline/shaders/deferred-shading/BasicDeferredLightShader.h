#pragma once

#include "pipeline\shaders\Shader.h"
#include "pipeline\shaders\deferred-shading\GBuffer.h"

namespace nTiled_pipeline {
	class BasicDeferredLightShader : public ShaderBatch {
	public:
		// constructor
		BasicDeferredLightShader(
			ShaderId shader_id,
			std::string path_geometry_pass_vertex_shader,
			std::string path_geometry_pass_fragment_shader,
			std::string path_light_pass_vertex_shader,
			std::string path_light_pass_fragment_shader,
			unsigned int width, unsigned int height
			);

		// render functions
		void init(Camera& camera);
		void render(Camera& camera);
		
		void renderGeometryPass(Camera& camera);
		void renderLightPass(Camera& camera);
		void renderBuffers();

		// Property functions
		ShaderId getId();
		void addObject(nTiled_world::Object& obj);
		void addLight(nTiled_world::PointLight* light);

		void loadShaders();

	private:
		const ShaderId id;

		std::vector<nTiled_world::PointLight*> lights;
		std::vector<PipelineLightData> light_data;

		std::vector<PipelineObject> objects;

		GLuint geometry_pass_sp;
		GLuint light_pass_sp;
		
		GLuint light_ubo;
		
		GBuffer gBuffer;

		std::string path_geometry_pass_vertex_shader;
		std::string path_geometry_pass_fragment_shader;

		std::string path_light_pass_vertex_shader;
		std::string path_light_pass_fragment_shader;

		PipelineObject fullscreen_quad;
	};
}
