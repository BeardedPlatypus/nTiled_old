#version 330

layout (location=0) in vec4 vertexPosition;
layout (location=1) in vec3 vertexColor;

out vec3 color;

uniform mat4 cameraToClipMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 modelToWorldMatrix;

void main() {
    color       = vertexColor;

    vec4 worldCoordinates = modelToWorldMatrix * vertexPosition;
    vec4 cameraCoordinates = worldToCameraMatrix * worldCoordinates;
    gl_Position = cameraToClipMatrix * cameraCoordinates;
}