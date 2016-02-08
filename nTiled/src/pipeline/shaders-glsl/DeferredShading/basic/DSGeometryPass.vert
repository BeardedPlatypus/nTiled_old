#version 440

// input 
// ----------------------------------------------------------------------------
layout (location=0) in vec4 vertexPosition;
layout (location=1) in vec3 vertexNormal;

// output
// ----------------------------------------------------------------------------
out vec4 cameraSpacePosition;
out vec3 cameraSpaceNormal;

// Variable definitions
// ----------------------------------------------------------------------------
// camera definition
uniform mat4 cameraToClipMatrix;
uniform mat4 modelToCameraMatrix;
uniform mat4 invTransModelToCameraMatrix;

// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
void main() {
	// calculate camera coordinates
    vec4 vertexCameraCoordinates = modelToCameraMatrix * vertexPosition;
	// calculate vertex clipspace coordinates
	gl_Position = cameraToClipMatrix * vertexCameraCoordinates;

	// pass values to fragment.
    cameraSpacePosition = vertexCameraCoordinates;
    cameraSpaceNormal = (invTransModelToCameraMatrix * vec4(vertexNormal, 0.0f)).xyz;
}