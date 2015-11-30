#version 450

layout (location=0) in vec4 vertexPosition;
layout (location=1) in vec3 vertexNormal; 

smooth out vec4 color;

uniform mat4 cameraToClipMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 modelToWorldMatrix;

uniform vec4 diffuseColor;
uniform vec4 lightIntensity;
uniform vec4 lightPosition;

void main() {
    vec4 vertexWorldCoordinates = modelToWorldMatrix * vertexPosition;
    vec4 vertexCameraCoordinates = worldToCameraMatrix * vertexWorldCoordinates;
    gl_Position = cameraToClipMatrix * vertexCameraCoordinates;

    vec4 lightCameraCoordinates = worldToCameraMatrix * lightPosition;
    vec3 directionToLight = normalize(vec3(lightCameraCoordinates - vertexCameraCoordinates));

    vec3 normalCameraCoordinates = normalize(mat3(worldToCameraMatrix) * vertexNormal);

    float cosAngIncidence = dot(normalCameraCoordinates, directionToLight);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);

    // color = lightIntensity * diffuseColor * cosAngIncidence;
    color = diffuseColor * cosAngIncidence;
}