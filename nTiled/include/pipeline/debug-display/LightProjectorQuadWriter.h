#pragma once

#include "pipeline\debug-display\DebugShader.h"
#include "pipeline\PipelineObject.h"
#include "state\State.h"
#include "pipeline\light-management\Tiled\LightProjector.h"

#include <vector>
#include <glm\glm.hpp>

namespace nTiled_pipeline {
	class LightProjectorQuadWriter : public DebugShader {
	public:
		LightProjectorQuadWriter(nTiled_state::State& state);
		void render();

	private:
		PipelineObject quad;
		GLuint position_buffer;

		GLuint shader_program;

		nTiled_state::State& state;
		BoxProjector projector;
	};
}
