#version 440

// Input buffers
// ----------------------------------------------------------------------------
layout (location=0) in vec3 pos;

uniform mat3 transform;

// Main
// ----------------------------------------------------------------------------
void main() {
    vec2 pos_transformed = (transform * vec3(pos.xy, 1.0)).xy;
    gl_Position = vec4(pos_transformed, pos.z, 1.0);
}
