#version 440

// Input buffers
// ----------------------------------------------------------------------------
layout (location=0) in vec4 vertexModelPosition;

// Output
// ----------------------------------------------------------------------------
out vec3 color;

// Variable
// ----------------------------------------------------------------------------
uniform mat4 modelToCameraMatrix;
uniform mat4 cameraToClipMatrix;
uniform vec3 core_color;

// Main
// ----------------------------------------------------------------------------
void main() {
    vec4 vertexCameraCoordinates = modelToCameraMatrix * vertexModelPosition;
	gl_Position = cameraToClipMatrix * vertexCameraCoordinates;
    color = vec3(1.0, 1.0, 1.0);
}
