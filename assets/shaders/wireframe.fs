#version 330

in vec4 interpolatedColor;
out vec4 fragmentColor;

void main() {
    fragmentColor = interpolatedColor;
}