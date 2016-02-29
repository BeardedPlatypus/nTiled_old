#pragma once

#include "pipeline\shaders\Shader.h"
#include "pipeline\light-management\Tiled\TiledManager.h"
#include "state\State.h"

namespace nTiled_pipeline {
	// ---------------------------------------------------------------------------
	//  Forward Light Abstract
	// ---------------------------------------------------------------------------
	class BasicForwardLightShader : public ShaderBatch {
	public:
		BasicForwardLightShader(ShaderId shader_id,
			                    std::string path_vertex_shader,
			                    std::string path_fragment_shader);
		ShaderId getId();

		void init(Camera& camera);
		virtual void render(Camera& camera) = 0;

		void addObject(nTiled_world::Object& obj);
		void addLight(nTiled_world::PointLight* light);

		virtual void loadShaders() = 0;
	protected:
		const ShaderId id;

		std::vector<nTiled_world::PointLight*> lights;
		std::vector<PipelineLightData> light_data;

		std::vector<PipelineObject> objects;

		GLuint shader_program;
		GLuint light_ubo;

		std::string path_vertex_shader;
		std::string path_fragment_shader;
	};
	// ---------------------------------------------------------------------------
	//  Solid Color Shader
	// ---------------------------------------------------------------------------
	class SolidShader : public BasicForwardLightShader {
	public:
		SolidShader(ShaderId shader_id,
			        std::string path_vertex_shader,
			        std::string path_fragment_shader);

		void init(Camera& camera);
		void render(Camera& camera);
		void loadShaders();
	};

	// ---------------------------------------------------------------------------
	//  Forward rendering: Vertex Lighting 
	// ---------------------------------------------------------------------------
	class BasicForwardVertLightShader : public BasicForwardLightShader {
	public:
		BasicForwardVertLightShader(ShaderId shader_id,
			                        std::string path_vertex_shader,
			                        std::string path_fragment_shader);
		void render(Camera& camera);
		void loadShaders();
	};

	// ---------------------------------------------------------------------------
	// Forward rendering: Fragment Lighting
	// ---------------------------------------------------------------------------
	class BasicForwardFragLightShader : public BasicForwardLightShader {
	public:
		BasicForwardFragLightShader(ShaderId shader_id,
                 			        std::string path_vertex_shader,
			                        std::string path_fragment_shader);
		void render(Camera& camera);
		void loadShaders();
	};

	// ---------------------------------------------------------------------------
	// Forward rendering: Tiled Lighting
	// ---------------------------------------------------------------------------
	class ForwardTiledLightShader : public BasicForwardLightShader {
	public:
		ForwardTiledLightShader(ShaderId shader_id, 
			                    std::string path_vertex_shader,
			                    std::string path_fragment_shader,
			                    glm::uvec2 tile_size,
			                    nTiled_state::State& state);

		void init(Camera& camera);
		void render(Camera& camera);
		void loadShaders();
	private:
		BoxProjector projector;
		TiledLightManager light_manager;

		GLuint light_grid_buffer;
		GLuint light_index_buffer;
	};
}
