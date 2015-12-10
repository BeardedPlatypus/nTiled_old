#pragma once

#include "pipeline\shaders\Shader.h"

namespace nTiled_pipeline {
	class BasicForwardLightShader : public ShaderBatch {
	public:
		BasicForwardLightShader(std::string name,
			std::string path_vertex_shader,
			std::string path_fragment_shader);
		std::string getName();

		void init(Camera& camera);
		virtual void render(Camera& camera) = 0;

		void addObject(PipelineObject* obj_p);
		void addLight(nTiled_world::PointLight* light);

		virtual void loadShaders() = 0;
	protected:
		const std::string name;
		std::vector<PipelineObject*> obj_ps;

		std::vector<nTiled_world::PointLight*> lights;
		std::vector<PipelineLightData> light_data;

		GLuint shader_program;
		GLuint light_ubo;

		std::string path_vertex_shader;
		std::string path_fragment_shader;
	};

	class BasicForwardVertLightShader : public BasicForwardLightShader {
	public:
		BasicForwardVertLightShader(std::string name,
			                        std::string path_vertex_shader,
			                        std::string path_fragment_shader);

		void render(Camera& camera);
		void loadShaders();
	};

	class BasicForwardFragLightShader : public BasicForwardLightShader {
	public:
		BasicForwardFragLightShader(std::string name,
			std::string path_vertex_shader,
			std::string path_fragment_shader);
		void render(Camera& camera);
		void loadShaders();
	};
}
