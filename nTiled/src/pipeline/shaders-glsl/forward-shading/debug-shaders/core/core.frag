#version 440

in vec3 color;
out vec4 fragmentColor;

//uniform vec3 core_color;

void main() {
    fragmentColor = vec4(1.0, 1.0, 1.0, 1.0); //core_color, 1.0);
}
