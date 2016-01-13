#pragma once

#include <glm\glm.hpp>


struct CameraData {
public:
	// constructor
	CameraData(glm::mat4 lookAt, 
		       glm::mat4 perspective_matrix, 
		       glm::vec2 depthrange);

	// member variables
	glm::mat4 lookAt;
	glm::mat4 perspective_matrix;
	glm::vec2 depthrange;
};