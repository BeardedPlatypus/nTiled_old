#include "gui\GuiElement.h"

#include <string>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace nTiled_gui;
CameraElement::CameraElement(Camera& camera) : camera(camera) {}

void CameraElement::render() {
	ImGui::Text("Camera");
	/*
	glm::mat4 lookAt = this->camera.getLookAt();
	std::string lookAt_string = glm::to_string(lookAt);
	std::cout << lookAt_string << std::endl;
	*/
}                        