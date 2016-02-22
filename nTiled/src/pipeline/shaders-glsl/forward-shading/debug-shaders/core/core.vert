#version 440

// Input buffers
// ----------------------------------------------------------------------------
layout (location=0) in vec4 vertexModelPosition;


// Variable
// ----------------------------------------------------------------------------
uniform mat4 modelToCameraMatrix;
uniform mat4 cameraToClipMatrix;

// Main
// ----------------------------------------------------------------------------
void main() {
    vec4 vertexCameraCoordinates = modelToCameraMatrix * vertexModelPosition;
	gl_Position = cameraToClipMatrix * vertexCameraCoordinates;
}