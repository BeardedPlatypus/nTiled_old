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
    
    vec3 lightAcc = vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < NUM_LIGHTS; i++) {
        vec3 lightDir = normalize(vec3(lights[i].positionModelSpace - modelSpacePosition));
        float cosAngIncidence = dot(normalizedNormal, lightDir);

        cosAngIncidence = clamp(cosAngIncidence, 0, 1);
        lightAcc += lights[i].intensity * cosAngIncidence;
    }
    fragmentColor = vec4((vec3(0.1f, 0.1f, 0.1f) + (lightAcc * 0.9)), 1.0);
}