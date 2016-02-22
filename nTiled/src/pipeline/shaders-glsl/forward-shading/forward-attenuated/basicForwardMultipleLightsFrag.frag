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
        vec3 L = vec3(lights[i].positionModelSpace - modelSpacePosition);
        float d = length(L);
        
        if (d < lights[i].radius) {
            vec3 lightDir = L / d;
			
			// Distance attenuation 
			//float attenuation = 1 ;/// (d * d);

            // Scale and bias attenuation:
            // attenuation = 0 at radius
            // attenuation = 1 a t d=0
			//attenuation = (attenuation - lights[i].radius) / (1.0f - lights[i].radius);
            //attenuation = max(attenuation, 0);
			float attenuation = clamp(1.0 - (d * d) / (lights[i].radius * lights[i].radius), 0.0, 1.0);
			attenuation *= attenuation;

            // cos Angle Incidence
            float cosAngIncidence = dot(normalizedNormal, lightDir);
            cosAngIncidence = clamp(cosAngIncidence, 0, 1);
			
			lightAcc += lights[i].intensity * cosAngIncidence * attenuation;
		}
    }
    fragmentColor = vec4((vec3(0.2f, 0.2f, 0.2f) + (lightAcc * 0.8)), 1.0);
}