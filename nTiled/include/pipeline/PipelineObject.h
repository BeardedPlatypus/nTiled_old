#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

#include "world\Lights.h"

// ---------------------------------------------------------------------------
// Pipeline data structs
namespace nTiled_pipeline {
	struct PipelineObject {
	public:
		// constructor
		PipelineObject(GLuint vao,
			GLuint element_buffer,
			GLuint element_buffer_size,
			glm::mat4 transformation);

		// member variables
		GLuint vao;
		GLuint element_buffer;
		GLuint element_buffer_size;
		glm::mat4 transformation;
	};

	struct PipelineLightData {
		glm::vec4 positionCameraCoordinates;
		glm::vec3 intensity;
		float radius;
		bool is_emitting;
	};
}

