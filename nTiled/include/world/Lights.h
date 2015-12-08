#pragma once

#include <glm\glm.hpp>

#include "world\Object.h"

namespace nTiled_world {
	struct PointLight {
	public:
		// Constructor
		PointLight(glm::vec4 position,
			       glm::vec3 intensity,
			       float radius,
			       Object& core,
			       Object& light_volume);
		PointLight(glm::vec4 position,
			       glm::vec3 intensity,
			       float radius,
			       bool isEmitting,
			       Object& core,
			       Object& light_volume);

		// Properties
		glm::vec4 position;
		float cutoff_radius;

		glm::vec3 intensity;
		bool is_emitting;

		Object& core;
		Object& light_volume;
	};
}
