#version 450

layout (location=0) in vec4 vertexPosition;
layout (location=1) in vec3 vertexNormal; 

smooth out vec3 color;

uniform mat4 cameraToClipMatrix;

//uniform mat4 modelToWorldMatrix;
//uniform mat4 worldToCameraMatrix;
uniform mat4 modelToCameraMatrix;
uniform mat3 normalModelToCameraMatrix;

uniform vec3 diffuseColor;
//uniform vec4 lightIntensity;
uniform vec4 lightCameraCoordinates;

void main() {
	// calculate camera coordinates
	//mat4 modelToCameraMatrix = worldToCameraMatrix * modelToWorldMatrix;
    vec4 vertexCameraCoordinates = modelToCameraMatrix * vertexPosition;
	vec3 normalCameraCoordinates = normalize(normalModelToCameraMatrix * vertexNormal);
	//vec3 normalCameraCoordinates = normalize(mat3(modelToCameraMatrix) * vertexNormal);
    
	// calculate vertex clipspace coordinates
	gl_Position = cameraToClipMatrix * vertexCameraCoordinates;

	// calculate lighting
    vec3 directionToLight = normalize(vec3(lightCameraCoordinates - vertexCameraCoordinates));
    float cosAngIncidence = dot(normalCameraCoordinates, directionToLight);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);

	

    //color = lightIntensity * diffuseColor * cosAngIncidence;
	color = vec3(0.1f, 0.1f, 0.1f) + (diffuseColor * cosAngIncidence *0.9);
}