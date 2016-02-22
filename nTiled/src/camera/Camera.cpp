#include "camera\Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "camera\CameraData.h"
// ----------------------------------------------------------------------------
//  Camera
// ----------------------------------------------------------------------------
//  Constructors
/*
Camera::Camera(CameraControl& control, CameraData data) : control(&control), 
                                                          data(data) {}



Camera::Camera(CameraControl& control, glm::vec3 camera_eye,
	                                   glm::vec3 camera_center,
	                                   glm::vec3 camera_up,
	                                   glm::mat4 perspective_matrix) : 
	Camera(control, CameraData(glm::lookAt(camera_eye,
		                                   camera_center,
		                                   camera_up), 
			                   perspective_matrix)) {}

Camera::Camera(CameraControl& control, glm::mat4 lookAt_matrix, 
	                                   float fovy,
	                                   float aspect,
	                                   float z_near,
	                                   float z_far) :
	Camera(control, CameraData(lookAt_matrix, glm::perspective(fovy, 
		                                                       aspect, 
		                                                       z_near, 
		                                                       z_far))) {}

*/
Camera::Camera(CameraControl* control, 
	           glm::vec3 camera_eye,
	           glm::vec3 camera_center,
	           glm::vec3 camera_up,
	           float fovy,
	           float aspect,
	           float z_near,
	           float z_far) :
	control(new TurnTableCameraControl()),
	data(CameraData(glm::lookAt(camera_eye,
		                        camera_center,
		                        camera_up), 
		            glm::perspective(fovy, 
		 			                 aspect, 
								     z_near, 
								     z_far),
		            glm::vec2(z_near, z_far))) {}

Camera::Camera() : Camera::Camera(new TurnTableCameraControl(),
	                              glm::vec3(-1.0, 0.0, 0.0),
	                              glm::vec3(0.0),
	                              glm::vec3(0.0, 1.0, 0.0),
	                              45.0,
	                              1.0,
	                              1.0,
	                              50.0) {}

/*
Camera::~Camera() {
	delete this->control;
}
*/

// ----------------------------------------------------------------------------
glm::mat4 Camera::getLookAt() {
	return this->data.lookAt;
}

glm::mat4 Camera::getPerspectiveMatrix() {
	return this->data.perspective_matrix;
}

glm::vec2 Camera::getDepthrange() {
	return this->data.depthrange;
}

void Camera::update(ImGuiIO& io) {
	this->control->update(this->data, io);
}

void Camera::toFocus(ImGuiIO& io) {
	this->control->activate(io);
}