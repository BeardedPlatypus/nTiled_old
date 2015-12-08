#version 450

// input buffers
// ----------------------------------------------------------------------------
layout (location=0) in vec4 vertexPosition;
layout (location=1) in vec3 vertexNormal; 

// output
// ----------------------------------------------------------------------------
smooth out vec3 color;

// Variable definitions
// ----------------------------------------------------------------------------
// camera definition
uniform mat4 cameraToClipMatrix;

uniform mat4 modelToCameraMatrix;
uniform mat3 normalModelToCameraMatrix;

// light definition
layout (std140) uniform LightBlock {
    vec4 positionCameraCoordinates;
    vec3 intensity;
    float radius;
    bool is_emitting;    
} light;

// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
void main() {
	// calculate camera coordinates
	//mat4 modelToCameraMatrix = worldToCameraMatrix * modelToWorldMatrix;
    vec4 vertexCameraCoordinates = modelToCameraMatrix * vertexPosition;
	vec3 normalCameraCoordinates = normalize(normalModelToCameraMatrix * vertexNormal);
    
	// calculate vertex clipspace coordinates
	gl_Position = cameraToClipMatrix * vertexCameraCoordinates;

	// calculate lighting
    vec3 directionToLight = normalize(vec3(light.positionCameraCoordinates - vertexCameraCoordinates));
    float cosAngIncidence = dot(normalCameraCoordinates, directionToLight);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);

    //color = lightIntensity * diffuseColor * cosAngIncidence;
	color = vec3(0.1f, 0.1f, 0.1f) + (light.intensity * cosAngIncidence *0.9);
}