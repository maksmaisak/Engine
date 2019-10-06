#version 330

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec4 color;

out vec4 vertexColor;

uniform mat4 matrixPVM;

void main() {

    gl_Position = matrixPVM * vec4(vertex, 1);
    vertexColor = color;
}