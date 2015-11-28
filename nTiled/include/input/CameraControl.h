#pragma once

#include "camera\Camera.h"
#include <GLFW\glfw3.h>

/*
 * Camera Control Interface
 */
class CameraControl {
public:
	virtual ~CameraControl() {}
	virtual void update(CameraData &data, GLFWwindow &window) = 0;
};


class TurnTableCameraControl : CameraControl {
public:
	//TODO: figure out a smarter way to deal with window instead of passing 
	// the whole reference to the window.
	TurnTableCameraControl();
	TurnTableCameraControl(double sensitivity);
	TurnTableCameraControl(bool was_pressed,
		                   double pos_x, double pos_y,
		                   double sensitivity);
	void update(CameraData &data, GLFWwindow &window);

private:
	bool was_pressed;
	double pos_prev_x, pos_prev_y;
	double sensitivity;
};