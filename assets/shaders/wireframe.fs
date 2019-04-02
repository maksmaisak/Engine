#version 330

in vec2 texCoords;
uniform vec4 color;
out vec4 fragmentColor;

void main() {

    fragmentColor = color + vec4(texCoords, 0, 0) * 0.00001;
}