#version 330

layout(location=0) in vec3 vertex;

uniform mat4 matrixPV;

out vec3 texCoords;

void main() {

    texCoords = vertex;

    vec4 pos = matrixPV * vec4(vertex, 1);
    gl_Position = pos.xyww;
}