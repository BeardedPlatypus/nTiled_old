#pragma once

#include <GLFW\glfw3.h>

#include "camera\CameraData.h"
#include <imgui.h>

/*
 * Camera Control Interface
 */
class CameraControl {
public:
	virtual ~CameraControl() {}
	virtual void update(CameraData& data, ImGuiIO& io) = 0;
	virtual void activate(ImGuiIO& io) = 0;
};


class TurnTableCameraControl : public CameraControl {
public:
	//TODO: figure out a smarter way to deal with window instead of passing 
	// the whole reference to the window.
	// ---------------------------------------------------------------------------
	//  Constructors
	// ---------------------------------------------------------------------------
	TurnTableCameraControl();
	TurnTableCameraControl(double sensitivity);
	
	// ---------------------------------------------------------------------------
	//  Member functions
	// ---------------------------------------------------------------------------
	void update(CameraData& data, ImGuiIO& io);
	void activate(ImGuiIO& io);

private:
	double sensitivity;
	ImVec2 pos_prev;
};