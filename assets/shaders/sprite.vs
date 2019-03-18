#version 330

layout(location=0) in vec3 vertex;
layout(location=1) in vec2 uv;

uniform mat4 matrixProjection;

out vec2 texCoords;

void main() {

    gl_Position = matrixProjection * vec4(vertex, 1);
    texCoords = uv;
}