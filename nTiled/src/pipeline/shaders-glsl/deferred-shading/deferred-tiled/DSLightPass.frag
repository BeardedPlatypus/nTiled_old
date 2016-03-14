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
// Tiled Shading structures
layout (std430, binding = 0) buffer LightGridBuffer {
	uvec2 tiles[];
};

layout (std430, binding = 1) buffer LightIndexBuffer {
	uint light_indices[];
};

uniform uvec2 tileSize;
uniform uint n_tiles_x;

// ----------------------------------------------------------------------------
//  Position definition
uniform mat4 perspectiveMatrix;
uniform mat4 invPerspectiveMatrix;

uniform vec4 viewport;
uniform vec2 depthrange;

//  Function definitions
// ----------------------------------------------------------------------------

vec2 calcTextureCoordinates() {
    return gl_FragCoord.xy / viewport.zw;
}

vec4 getCameraCoordinates(in vec2 texCoords) {
    // Define the normalized device coordinates
    vec3 device;
    device.xy = (2.0f * ((gl_FragCoord.xy - vec2(0.5f, 0.5f) - viewport.xy) / viewport.zw)) - 1.0f;
    device.z = 2.0f * texture(depthTexture, texCoords).x - 1.0f;

    // Calculate actual coordinates
    vec4 rawCoords = invPerspectiveMatrix * vec4(device, 1.0f);
    vec4 coords = rawCoords / rawCoords.w;

    return coords;
}

// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
void main() {
    // calculate position.
    // ------------------------------------------------------------------------
  
    vec2 texCoords = calcTextureCoordinates();
    vec4 fragmentCameraPosition = getCameraCoordinates(texCoords);

    // calculate lighting
    // ------------------------------------------------------------------------
    vec3 normalizedNormal = normalize(texture(normalTexture, texCoords).xyz);
    vec3 diffuseColor = texture(diffuseTexture, texCoords).xyz;

    if (diffuseColor.rgb == vec3(0.0f, 0.0f, 0.0f)) {
        fragmentColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    } 
    else {
        // Tiled Shading 
        vec2 position = gl_FragCoord.xy - vec2(0.5, 0.5);  

        int tile_index = int(floor(position.x / tileSize.x) + 
                             n_tiles_x * floor(position.y / tileSize.y));
 
        uint offset = tiles[tile_index].x;
        uint n_lights = tiles[tile_index].y;

        // Shading
        vec3 lightAcc = vec3(0.0f, 0.0f, 0.0f);
        for (uint i = offset; i < offset + n_lights; i++) {
            Light light = lights[light_indices[i]];

            vec3 L = vec3(light.positionCameraSpace - fragmentCameraPosition);
            float d = length(L);

            if (d < light.radius) {
                vec3 lightDir = L / d;

			    float attenuation = clamp(1.0 - (d * d) / (light.radius * light.radius), 0.0, 1.0);
			    attenuation *= attenuation;
                
                float cosAngIncidence = dot(normalizedNormal, lightDir);
                cosAngIncidence = clamp(cosAngIncidence, 0, 1);
			
			    lightAcc += light.intensity * cosAngIncidence * attenuation;
            }
        }
   
        fragmentColor = vec4((vec3(0.1f, 0.1f, 0.1f) + (lightAcc * 0.9)), 1.0);
    }
}
