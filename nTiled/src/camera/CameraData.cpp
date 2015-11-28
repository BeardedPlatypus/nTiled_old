#include "camera\CameraData.h"

// ----------------------------------------------------------------------------
//  CameraData
// ----------------------------------------------------------------------------
CameraData::CameraData(glm::mat4 lookAt,
	                   glm::mat4 perspective) :
	lookAt(lookAt),
	perspective_matrix(perspective) {}
