#pragma once

#include <glad\glad.h>
#include <string>

#include "camera\Camera.h"

namespace nTiled_pipeline {
	class Pipeline {
	public:
		// Render Methods
		virtual void init() = 0;
		virtual void render() = 0;

		// Property Methods
		virtual Camera getActiveCamera() = 0;
		virtual void setActiveCamera(Camera& camera) = 0;
	};

	class ForwardPipeline : Pipeline {
	public:
		ForwardPipeline(Camera& camera, 
			            const std::string vertex_shader_path,
			            const std::string fragment_shader_path);

		// Render Methods
		void init();
		void render();

		// Property Methods
		Camera getActiveCamera();
		void setActiveCamera(Camera& camera);

		void loadShaderProgram(const std::string vertex_shader_path,
			                   const std::string fragment_shader_path);

	private:
		Camera* camera;

		std::string vertex_shader_path;
		std::string fragment_shader_path;
		GLuint shader_program;
	};
}
