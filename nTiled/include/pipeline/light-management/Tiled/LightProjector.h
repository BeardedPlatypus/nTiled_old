#pragma once

#include "world\Lights.h"
#include "camera\Camera.h"
#include "state\State.h"

#include <vector>

namespace nTiled_pipeline {
	// ------------------------------------------------------------------------
	//  LightProjectionCompute classes
	// ------------------------------------------------------------------------
	class LightProjector {
	public:
		virtual ~LightProjector() {}
		virtual bool computeProjection(
			nTiled_world::PointLight& light,
			Camera& camera,
			glm::uvec2 viewport,
			glm::uvec2 tilesize,
			glm::uvec4& projection) = 0;
	};

	class BoxProjector : public LightProjector {
	public:
		// FIXME add better clipping
		// constructor
		BoxProjector();

		// compute method
		bool computeProjection(
			nTiled_world::PointLight& light,  // input
			Camera& camera,
			glm::uvec2 viewport,
			glm::uvec2 tilesize,
			glm::uvec4& projection);          // output

		bool computeNDCProjection(
			// Input
			nTiled_world::PointLight& light,
			Camera& camera,
			// Output
			glm::vec4& ndc_coordinates);

		bool compute2dProjection(
			// Input
			glm::vec2 pos_cameraspace,
			float cutoff_radius,
			// Output
			glm::vec2& B_star,
			glm::vec2& T_star);
	};
}

