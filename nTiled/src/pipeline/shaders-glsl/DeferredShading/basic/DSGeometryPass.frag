#version 440

// input buffers
// ----------------------------------------------------------------------------
in vec4 cameraSpacePosition;
in vec3 cameraSpaceNormal;

// output
// ----------------------------------------------------------------------------
//out vec3 diffuseOut;
layout (location = 0) out vec3 diffuseOut; 
layout (location = 1) out vec3 normalOut; 

// Variable definitions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
void main() {
    diffuseOut = vec3(1.0f, 1.0f, 1.0f); // This should be replaced with actual
                                         // diffuse once textures are supported
    normalOut = normalize(cameraSpaceNormal);
}