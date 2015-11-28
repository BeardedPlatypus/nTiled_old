#include "camera\Camera.h"

#include <glm/gtc/matrix_transform.hpp>
// ----------------------------------------------------------------------------
//  CameraData
// ----------------------------------------------------------------------------
CameraData::CameraData(glm::mat4 lookAt,
	                   glm::mat4 perspective) : 
	lookAt(lookAt),
	perspective_matrix(perspective) {}

// ----------------------------------------------------------------------------
//  Camera
// ----------------------------------------------------------------------------
Camera::Camera(CameraControl &control, CameraData data) : control(control), 
                                                          data(data) {}



Camera::Camera(CameraControl &control, glm::vec3 camera_eye,
	                                   glm::vec3 camera_center,
	                                   glm::vec3 camera_up,
	                                   glm::mat4 perspective_matrix) : 
	Camera(control, CameraData(glm::lookAt(camera_eye,
		                                   camera_center,
		                                   camera_up), 
			                   perspective_matrix)) {}

Camera::Camera(CameraControl &control, glm::mat4 lookAt_matrix, 
	                                   float fovy,
	                                   float aspect,
	                                   float z_near,
	                                   float z_far) :
	Camera(control, CameraData(lookAt_matrix, glm::perspective(fovy, aspect, z_near, z_far))) {}


Camera::Camera(CameraControl &control, glm::vec3 camera_eye,
	                                   glm::vec3 camera_center,
	                                   glm::vec3 camera_up,
	                                   float fovy,
	                                   float aspect,
	                                   float z_near,
	                                   float z_far) :
	Camera(control, CameraData(glm::lookAt(camera_eye,
		                                   camera_center,
		                                   camera_up), 
		                       glm::perspective(fovy, 
								                aspect, 
								                z_near, 
								                z_far))) {}


glm::mat4 Camera::getLookAt() {
	return this->data.lookAt;
}

glm::mat4 Camera::getPerspectiveMatrix() {
	return this->data.perspective_matrix;
}

void Camera::update() {
	this->control.update(this->data);
}