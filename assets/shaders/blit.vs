#version 330

layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;

void main() {

    gl_Position = vec4(inVertex, 1.f);
    texCoords = inTexCoords;
}
