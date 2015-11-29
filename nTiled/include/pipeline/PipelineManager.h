#pragma once

#include <glad\glad.h>
#include "pipeline\Pipeline.h"
#include "world\World.h"

namespace nTiled_pipeline {
	class PipelineManager {
	public:
		PipelineManager(Pipeline& pipeline, nTiled_world::World& world);

		void init();
		void render();
	private:
		Pipeline* pipeline;
		nTiled_world::World* world;

		GLuint vao;
		GLuint element_buffer;
	};
}