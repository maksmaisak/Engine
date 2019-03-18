#version 330

layout(location=0) in vec3 vertex;

uniform	mat4 matrixModel;

void main() {

    gl_Position = matrixModel * vec4(vertex, 1);
}
