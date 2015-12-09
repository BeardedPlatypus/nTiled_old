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
		glm::vec4 positionCoordinates; 
		glm::vec3 intensity;                 
		float radius;                        
		int is_emitting;                    
		int _pad[3]; // Add 3 padding to align PipeLineLightData for gpu
	};
}

