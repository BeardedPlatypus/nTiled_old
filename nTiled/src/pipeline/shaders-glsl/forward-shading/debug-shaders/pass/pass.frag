#version 440

//  output
// ----------------------------------------------------------------------------
out vec4 fragmentColor;



layout(binding = 0) uniform sampler2D resultTexture;
uniform vec4 viewport;

vec2 calcTextureCoordinates() {
    return gl_FragCoord.xy / viewport.zw;
}

void main() {
    vec2 texCoords = calcTextureCoordinates();
    fragmentColor = vec4(texture(resultTexture, texCoords).xyz, 1.0);
}