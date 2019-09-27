#version 330

uniform	mat4 matrixPVM;

in vec3 vertex;
in vec2 uv;

out vec2 texCoord;

void main (void) {

    gl_Position = matrixPVM * vec4(vertex, 1.f);
    texCoord = uv;
}
