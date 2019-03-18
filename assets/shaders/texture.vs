//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 matrixPVM;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated

void main (void) {

    gl_Position = matrixPVM * vec4(vertex, 1.f);
	texCoord = uv;
}
