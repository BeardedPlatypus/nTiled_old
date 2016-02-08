#version 440

//#define MAX_N_LIGHTS 20
#define NUM_LIGHTS 0

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
struct Light {
    vec4 positionCameraCoordinates; // 4
    vec3 intensity;                 // 3
    float radius;                   // 1
    int is_emitting;
};

//uniform int numLights;    
layout (std140) uniform LightBlock {
    Light lights[NUM_LIGHTS];
};

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
    vec3 lightAcc = vec3(0.0f, 0.0f, 0.0f);

    //for (int i = 0; i < numLights; i++) {
    // due to dynamic for loops not working, (ab)using preprocessor instead.
    for (int i = 0; i < NUM_LIGHTS; i++) {
        vec3 directionToLight = normalize(vec3(lights[i].positionCameraCoordinates - vertexCameraCoordinates));
        float cosAngIncidence = dot(normalCameraCoordinates, directionToLight);
        cosAngIncidence = clamp(cosAngIncidence, 0, 1);

	    lightAcc += lights[i].intensity * cosAngIncidence;
    }
    color = vec3(0.1f, 0.1f, 0.1f) + (lightAcc * 0.9);
}