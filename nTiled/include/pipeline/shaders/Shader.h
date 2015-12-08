#pragma once

#include <glad\glad.h>

#include <vector>

#include "camera\Camera.h"
#include "pipeline\PipelineObject.h"
#include "world\Lights.h"


namespace nTiled_pipeline {
	class ShaderBatch {
	public:
		virtual ~ShaderBatch() {}
		virtual std::string getName() = 0;

		virtual void init(Camera& camera) = 0;
		virtual void render(Camera& camera) = 0;

		virtual void addObject(PipelineObject* obj_p) = 0;
		virtual void setLight(nTiled_world::PointLight* light) = 0;
	};

	class BasicForwardVertLightShader : public ShaderBatch {
	public:
		// TODO: add pipelineLight object?
		BasicForwardVertLightShader(std::string name,
			                        std::string path_vertex_shader,
			                        std::string path_fragment_shader);
		std::string getName();

		void init(Camera& camera);
		void render(Camera& camera);

		void addObject(PipelineObject* obj_p);
		void setLight(nTiled_world::PointLight* light);
		
	private:
		const std::string name;
		std::vector<PipelineObject*> obj_ps;
		
		nTiled_world::PointLight* light;
		PipelineLightData light_data;

		GLuint shader_program;
		GLuint light_ubo;

		std::string path_vertex_shader;
		std::string path_fragment_shader;
	};
}