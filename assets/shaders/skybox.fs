#version 330

in vec3 texCoords;

uniform samplerCube skyboxTexture;

out vec4 color;

void main() {

    color = texture(skyboxTexture, texCoords);
}  