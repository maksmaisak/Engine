//DIFFUSE COLOR VERTEX SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

layout(location = 0) in vec3 vertex;

uniform	mat4 matrixProjection;
uniform	mat4 matrixView;
uniform	mat4 matrixModel;

void main(void) {

    gl_Position = matrixProjection * matrixView * matrixModel * vec4(vertex, 1.f);
}
