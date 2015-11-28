#include "input\CameraControl.h"

// ----------------------------------------------------------------------------
//  System Libraries
// ----------------------------------------------------------------------------
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

// ----------------------------------------------------------------------------
//  TurnTableControl
// ----------------------------------------------------------------------------
//  Constructors
TurnTableCameraControl::TurnTableCameraControl(bool was_pressed, 
	                                           double pos_x,
	                                           double pos_y,
	                                           double sensitivity) : 
	was_pressed(was_pressed),
	pos_prev_x(pos_x),
	pos_prev_y(pos_y),
	sensitivity(sensitivity) {}

TurnTableCameraControl::TurnTableCameraControl(double sensitivity) :
	TurnTableCameraControl(false, 0.0, 0.0, sensitivity) {}

TurnTableCameraControl::TurnTableCameraControl() :
	TurnTableCameraControl(false, 0.0, 0.0, 320.0) {}

// ----------------------------------------------------------------------------
//  Methods
void TurnTableCameraControl::update(CameraData &data, GLFWwindow &window) {
	int state = glfwGetMouseButton(&window, GLFW_MOUSE_BUTTON_LEFT);

	if (state == GLFW_PRESS) {
		if (was_pressed) {
			// y values map to camera space x
			// x values map to world space y
			double pos_cur_x, pos_cur_y;
			glfwGetCursorPos(&window, &pos_cur_x, &pos_cur_y);

			// camera x-axis rotations
			double dif_y = (pos_cur_y - this->pos_prev_y);
			float angle_x = dif_y / this->sensitivity;

			glm::mat4 lookAt = data.lookAt;
			glm::vec3 camera_x_axis = glm::vec3(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) * lookAt);
			lookAt = glm::rotate(lookAt, angle_x, camera_x_axis);

			// world y-axis rotations
			double dif_x = (pos_cur_x - this->pos_prev_x);
			float angle_y = dif_x / this->sensitivity;
			lookAt = glm::rotate(lookAt, angle_y, glm::vec3(0.0f, 1.0f, 0.0f));

			// update variables
			data.lookAt = lookAt;
			this->pos_prev_y = pos_cur_y;
			this->pos_prev_x = pos_cur_x;
		}
		else {
			this->was_pressed = true;
			glfwGetCursorPos(&window, &this->pos_prev_x, &this->pos_prev_y);
		}
	}
	else {
		was_pressed = false;
	}
}