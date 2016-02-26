#include "pipeline\light-management\Tiled\LightProjector.h"

#include <cmath>

// ----------------------------------------------------------------------------
//  System Libraries
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
using namespace nTiled_pipeline;

// ----------------------------------------------------------------------------
//  LightProjector class
// ----------------------------------------------------------------------------
// constructor 
// TODO: add option for more options than just square projections

BoxProjector::BoxProjector() {}

bool BoxProjector::computeProjection(
	nTiled_world::PointLight& light,
	Camera& camera,
	glm::uvec2 viewport,
	glm::uvec2 tilesize,
	glm::uvec4& projection) {


	glm::vec4 ndc_coordinates;
	if (computeNDCProjection(light, camera, ndc_coordinates)) {
		// FIXME CONFIRM I USE SAME INDICES EVERYWHERE
		// compute position in pixel coordinates
		projection.x = floor(((ndc_coordinates.x + 1.0f) * 0.5f) * viewport.x) / tilesize.x;
		projection.y = floor(((ndc_coordinates.y + 1.0f) * 0.5f) * viewport.y) / tilesize.y;
		projection.z = floor(((ndc_coordinates.z + 1.0f) * 0.5f) * viewport.x) / tilesize.x;
		projection.w = floor(((ndc_coordinates.w + 1.0f) * 0.5f) * viewport.y) / tilesize.y;

		return true;
	}
	return false;
}

bool BoxProjector::computeNDCProjection(
	// input
	nTiled_world::PointLight& light,
 	Camera& camera,
	// output
	glm::vec4& ndc_coordinates) {
	// Calculate Light Position in Camera space
	// ----------------------------------------------------------------------------------
	const glm::vec3 light_position = glm::vec3(camera.getLookAt() * light.position);

	// cull on z-axis
	// FIXME quick fix to fix signs of axis this really should be fixed across my application
	if ((light_position.z - light.cutoff_radius > -camera.getDepthrange().x) ||
		(light_position.z + light.cutoff_radius < -camera.getDepthrange().y))
		return false;

	// calculate x_axis
	// ----------------------------------------------------------------------------------
	glm::vec2 xz_light_pos = glm::vec2(light_position.x, light_position.z);
	glm::vec2 ndc_bounding_x;

	glm::vec2 B_star;
	glm::vec2 T_star;

	if (!compute2dProjection( xz_light_pos,         // Input
	 	                      light.cutoff_radius,
		                      B_star, T_star        // Output
		                     )) {
		ndc_bounding_x = glm::vec2(-1.0, 1.0);
	}
	else {
		glm::vec4 proj_B = camera.getPerspectiveMatrix() * glm::vec4(B_star.x, 0.0f, B_star.y, 1.0);
		glm::vec4 ndc_B = proj_B / proj_B.w;

		glm::vec4 proj_T = camera.getPerspectiveMatrix() * glm::vec4(T_star.x, 0.0f, T_star.y, 1.0);
		glm::vec4 ndc_T = proj_T / proj_T.w;

		// light clipping
		if (ndc_T.x < -1.0 || ndc_B.x > 1.0)
			return false;

		ndc_bounding_x = glm::vec2(ndc_B.x, ndc_T.x);

		// FIXME check if this code make sense
		if (ndc_bounding_x.x > ndc_bounding_x.y ) {
			if (ndc_bounding_x.x > -1.0 && ndc_bounding_x.x < 1.0) {
				ndc_bounding_x.y = 1.0;
			}
			else if (ndc_bounding_x.y > -1.0 && ndc_bounding_x.y < 1.0) {
				ndc_bounding_x.x = -1.0;
			}
			else {
				return false;
			}
		}
	}

	// calculate y_axis
	// ----------------------------------------------------------------------------------
	glm::vec2 yz_light_pos = glm::vec2(light_position.y, light_position.z);
	glm::vec2 ndc_bounding_y;

	if (!compute2dProjection( yz_light_pos,         // Input
		                     light.cutoff_radius,
		                     B_star, T_star        // Output
		                    )) {
		ndc_bounding_y = glm::vec2(-1.0, 1.0);
	}
	else {
		glm::vec4 proj_B = camera.getPerspectiveMatrix() * glm::vec4(0.0, B_star.x, B_star.y, 1.0);
		glm::vec4 ndc_B = proj_B / proj_B.w;

		glm::vec4 proj_T = camera.getPerspectiveMatrix() * glm::vec4(0.0, T_star.x, T_star.y, 1.0);
		glm::vec4 ndc_T = proj_T / proj_T.w;

		// light clipping
		if (ndc_T.y < -1.0 || ndc_B.y > 1.0)
			return false;

		ndc_bounding_y = glm::vec2(ndc_B.y, ndc_T.y);

		// FIXME check if this code make sense
		if (ndc_bounding_y.x > ndc_bounding_y.y ) {
			if (ndc_bounding_y.x > -1.0 && ndc_bounding_y.x < 1.0) {
				ndc_bounding_y.y = 1.0;
			}
			else if (ndc_bounding_y.y > -1.0 && ndc_bounding_y.y < 1.0) {
				ndc_bounding_y.x = -1.0;
			}
			else {
				return false;
			}
		}
	}

	ndc_coordinates.x = ndc_bounding_x.x;
	ndc_coordinates.y = ndc_bounding_y.x;
	ndc_coordinates.z = ndc_bounding_x.y;
	ndc_coordinates.w = ndc_bounding_y.y;
	return true;
}


bool BoxProjector::compute2dProjection(
	// Input
	glm::vec2 pos_cameraspace,
	float cutoff_radius,
	// Output
	glm::vec2& B_star,
	glm::vec2& T_star) {

	// Origin is within light sphere
	if (abs(pos_cameraspace.x) < cutoff_radius &&
		abs(pos_cameraspace.y) < cutoff_radius) {
		return false;
	}
	// Origin is outside light_sphere
	else {
		float x = cutoff_radius / (sqrtf(pos_cameraspace.x * pos_cameraspace.x +
			                             pos_cameraspace.y * pos_cameraspace.y));
		float cos = 1.0 / (sqrtf(x * x + 1));
		float sin = x * cos;
		float min_sin = -1.0 * sin;

		glm::mat2 R_B = glm::mat2(glm::vec2(cos, min_sin), glm::vec2(sin, cos));
		glm::mat2 R_T = glm::mat2(glm::vec2(cos, sin), glm::vec2(min_sin, cos));
	
		B_star = R_B * pos_cameraspace;
		T_star = R_T * pos_cameraspace;
		return true;
	}
}

