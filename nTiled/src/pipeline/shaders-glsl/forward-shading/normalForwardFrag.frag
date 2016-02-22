
#version 440

#define NUM_LIGHTS 0

// input buffers
// ----------------------------------------------------------------------------
in vec4 modelSpacePosition;
in vec3 modelSpaceNormal;

// output
// ----------------------------------------------------------------------------
out vec4 fragmentColor;

// Variable definitions
// ----------------------------------------------------------------------------
// light definition
struct Light {
    vec4 positionModelSpace;            // 4
    vec3 intensity;                     // 3
    float radius;                       // 1
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
    vec3 normalizedNormal = normalize(modelSpaceNormal);
    fragmentColor = vec4(normalizedNormal , 1.0);
}
