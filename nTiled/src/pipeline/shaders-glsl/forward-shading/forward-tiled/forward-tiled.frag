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

// Tiled Shading structures
// ----------------------------------------------------------------------------
layout (std430, binding = 0) buffer LightGridBuffer {
	uvec2 tiles[];
};

layout (std430, binding = 1) buffer LightIndexBuffer {
	uint light_indices[];
};

// Position definition
// ----------------------------------------------------------------------------
uniform uvec2 tileSize;
uniform uint n_tiles_x;

// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
void main() {
    vec3 normalizedNormal = normalize(modelSpaceNormal);
   
    // determine light tile
    vec2 position = gl_FragCoord.xy - vec2(0.5, 0.5);  
    int tile_index = int(floor(position.x / tileSize.x) + 
                         n_tiles_x * floor(position.y / tileSize.y));
 
    uint offset = tiles[tile_index].x;
    uint n_lights = tiles[tile_index].y;

    vec3 lightAcc = vec3(0.0f, 0.0f, 0.0f);
    for (uint i = offset; i < offset + n_lights; i++) {
        // Look up the specific light
        Light light = lights[light_indices[i]];

        vec3 L = vec3(light.positionModelSpace - modelSpacePosition);
        float d = length(L);
        
        if (d < light.radius) {
            vec3 lightDir = L / d;
			
			// Distance attenuation 
			float attenuation = clamp(1.0 - (d * d) / (light.radius * light.radius), 0.0, 1.0);
			attenuation *= attenuation;

            // cos Angle Incidence
            float cosAngIncidence = dot(normalizedNormal, lightDir);
            cosAngIncidence = clamp(cosAngIncidence, 0, 1);
			
			lightAcc += light.intensity * cosAngIncidence * attenuation;
		}
    }
    fragmentColor = vec4((vec3(0.1f, 0.1f, 0.1f) + (lightAcc * 0.9)), 1.0);
}