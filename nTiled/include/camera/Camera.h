#pragma once

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include "camera\CameraControl.h"
#include "camera\CameraData.h"

class Camera {
public:
	/*
	Camera(CameraControl& control, CameraData data);

	Camera(CameraControl& control, glm::vec3 camera_eye,
								   glm::vec3 camera_center,
								   glm::vec3 camera_up,
								   glm::mat4 perspective_matrix);
	Camera(CameraControl& control, glm::mat4 lookAt_matrix,
								   float fovy,
								   float aspect,
								   float z_near,
								   float z_far);
	*/
	// Constructor
	Camera(CameraControl& control, glm::vec3 camera_eye,
								   glm::vec3 camera_center,
								   glm::vec3 camera_up,
								   float fovy,
								   float aspect,
								   float z_near,
								   float z_far);
	// Destructor
	//~Camera();

	// Access Camera Data
	glm::mat4 getLookAt();
	glm::mat4 getPerspectiveMatrix();
	glm::vec2 getDepthrange();

	// Update Camera data based on the io and the set CameraControl
	void update(ImGuiIO& io);
	void toFocus(ImGuiIO& io);

private:
	CameraControl* control;
	CameraData data;

	bool is_active;
};