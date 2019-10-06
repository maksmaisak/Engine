#version 330

layout (location = 0) in vec4 vertex; // xyz is position, w is line width in clipspace.
layout (location = 1) in vec4 color;

out vec4 vertexColor;
out float vertexLineHalfWidth;

uniform mat4 matrixPVM;

void main() {

    gl_Position = matrixPVM * vec4(vertex.xyz, 1);
    vertexColor = color;
    vertexLineHalfWidth = vertex.w;
}