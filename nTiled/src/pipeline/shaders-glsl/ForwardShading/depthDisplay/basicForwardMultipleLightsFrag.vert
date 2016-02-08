#version 440

// input buffers
// ----------------------------------------------------------------------------
layout (location=0) in vec4 vertexPosition;
layout (location=1) in vec3 vertexNormal; 

// output
// ----------------------------------------------------------------------------
out vec4 modelSpacePosition;
out vec3 modelSpaceNormal;

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

	// pass values to fragment.
    modelSpacePosition = vertexPosition;
    modelSpaceNormal = vertexNormal;
}