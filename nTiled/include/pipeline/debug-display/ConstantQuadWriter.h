#pragma once

#include "pipeline\debug-display\DebugShader.h"
#include "pipeline\PipelineObject.h"

#include <vector>
#include <glm\glm.hpp>

namespace nTiled_pipeline {
	class ConstantQuadWriter : public DebugShader {
	public:
		ConstantQuadWriter();
		void render();

		void addQuad(glm::vec4 ndc_coordinates);

	private:
		std::vector<PipelineObject> quads;

		GLuint shader_program;
	};
}
