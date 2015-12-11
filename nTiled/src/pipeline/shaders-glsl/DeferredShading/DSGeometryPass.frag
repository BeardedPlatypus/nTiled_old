#version 450

// input buffers
// ----------------------------------------------------------------------------
in vec4 modelSpacePosition;
in vec3 modelSpaceNormal;

// output
// ----------------------------------------------------------------------------
layout (location = 0) out vec3 diffuseOut; 
layout (location = 1) out vec3 normalOut; 

// Variable definitions
// ----------------------------------------------------------------------------
uniform sampler2D gColorMap; 

// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
void main() {
    diffuseOut = vec3(1.0f, 1.0f, 1.0f); // This should be replaced with actual
                                         // diffuse once textures are supported
    normalOut = modelSpaceNormal;
}