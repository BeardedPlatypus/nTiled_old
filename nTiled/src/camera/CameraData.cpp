#include "camera\CameraData.h"

// ----------------------------------------------------------------------------
//  CameraData
// ----------------------------------------------------------------------------
CameraData::CameraData(glm::mat4 lookAt,
	                   glm::mat4 perspective,
	                   glm::vec2 depthrange) :
	lookAt(lookAt),
	perspective_matrix(perspective),
	depthrange(depthrange) {}
