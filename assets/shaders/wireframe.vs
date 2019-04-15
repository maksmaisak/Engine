#version 330

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec4 color;

uniform mat4 matrixPVM;

out vec4 interpolatedColor;

void main() {
    gl_Position = matrixPVM * vec4(vertex, 1);
    interpolatedColor = color;
}