#pragma once

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include "input\CameraControl.h"
#include "camera\CameraData.h"

class Camera {
public:
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
	Camera(CameraControl& control, glm::vec3 camera_eye,
								   glm::vec3 camera_center,
								   glm::vec3 camera_up,
								   float fovy,
								   float aspect,
								   float z_near,
								   float z_far);

	glm::mat4 getLookAt();
	glm::mat4 getPerspectiveMatrix();

	void update(GLFWwindow& window);

private:
	CameraControl& control;
	CameraData data;

	bool is_active;
};