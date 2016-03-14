#version 440

#define NUM_LIGHTS 0

//  output
// ----------------------------------------------------------------------------
out vec4 fragmentColor;

//  Variable definitions
// ----------------------------------------------------------------------------
//  light definition
struct Light {
    vec4 positionCameraSpace;            // 4
    vec3 intensity;                      // 3
    float radius;                        // 1
    int is_emitting;
};

//uniform int numLights;    
layout (std140) uniform LightBlock {
    Light lights[NUM_LIGHTS];
};

// ----------------------------------------------------------------------------
//  MRT texture samplers
layout(binding = 0) uniform sampler2D diffuseTexture;
layout(binding = 1) uniform sampler2D normalTexture;
layout(binding = 2) uniform sampler2D depthTexture;

// ----------------------------------------------------------------------------
//  Position definition
uniform mat4 perspectiveMatrix;
uniform mat4 invPerspectiveMatrix;

uniform vec4 viewport;
uniform vec2 depthrange;

//  Function definitions
// ----------------------------------------------------------------------------
vec4 calcCameraPositionFromWindowPosition(in vec3 windowSpace) {
	vec3 ndcPos;
	ndcPos.xy = ((2.0 * windowSpace.xy) - (2.0 * viewport.xy)) / (viewport.zw) - 1;
	ndcPos.z = (2.0 * windowSpace.z - depthrange.x - depthrange.y) /
    (depthrange.y - depthrange.x);
 
	vec4 clipPos;
	clipPos.w = perspectiveMatrix[3][2] / 
                (ndcPos.z - (perspectiveMatrix[2][2] / perspectiveMatrix[2][3]));
	clipPos.xyz = ndcPos * clipPos.w;
 
	return invPerspectiveMatrix * clipPos;
}

vec2 calcTextureCoordinates() {
    return gl_FragCoord.xy / viewport.zw;
}

float linearizeDepth(in vec2 uv) {
  float z = texture(depthTexture, uv).x;
  return (2.0 * depthrange.x) / (depthrange.y + depthrange.x - z * (depthrange.y - depthrange.x));	
}

// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
void main() {
    // calculate position.
    // ------------------------------------------------------------------------
    vec2 texCoords = calcTextureCoordinates();
    vec3 W = vec3(gl_FragCoord.xy, texture(depthTexture, texCoords).x);
    
    vec4 fragmentCameraPosition = calcCameraPositionFromWindowPosition(W);

    // calculate lighting
    // ------------------------------------------------------------------------
    vec3 normalizedNormal = normalize(texture(normalTexture, texCoords).xyz);
    vec3 diffuseColor = texture(diffuseTexture, texCoords).xyz;

    if (diffuseColor.rgb == vec3(0.0f, 0.0f, 0.0f)) {
        fragmentColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    } 
    else {
        vec3 lightAcc = vec3(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < NUM_LIGHTS; i++) {
            vec3 L = vec3(lights[i].positionCameraSpace - fragmentCameraPosition);
            float d = length(L);
        
            if (d < lights[i].radius) {
                vec3 lightDir = L / d;
			
    			float attenuation = clamp(1.0 - (d * d) / (lights[i].radius * lights[i].radius), 0.0, 1.0);
    			attenuation *= attenuation;

                // cos Angle Incidence
                float cosAngIncidence = dot(normalizedNormal, lightDir);
                cosAngIncidence = clamp(cosAngIncidence, 0, 1);
			
			    lightAcc += lights[i].intensity * cosAngIncidence * attenuation;
            }
        }
        fragmentColor = vec4(cosAngIncidence, cosAngIncidence, cosAngIncidence, 1.0);
    }
}