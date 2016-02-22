#version 440

// input buffers
// ----------------------------------------------------------------------------
layout (location=0) in vec4 vertexPosition;

// Variable definitions
// ----------------------------------------------------------------------------
// camera definition
uniform mat4 cameraToClipMatrix;
uniform mat4 modelToCameraMatrix;

// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
void main() {
	// calculate camera coordinates
    vec4 vertexCameraCoordinates = modelToCameraMatrix * vertexPosition;
	// calculate vertex clipspace coordinates
	gl_Position = cameraToClipMatrix * vertexCameraCoordinates;
}
